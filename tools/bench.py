#!/usr/bin/env python3
"""Run perf stat once per case in /dev/shm; print one table without pinning."""
import json
import math
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

EVENTS = "task-clock,cycles,instructions,branches,branch-misses"


def number(counter, field="counter-value"):
    try:
        value = float(counter[field])
        return value if math.isfinite(value) and value >= 0 else None
    except (KeyError, TypeError, ValueError):
        return None


def main():
    if len(sys.argv) != 3:
        sys.exit("usage: bench.py BINARY PROBLEM_DIR")
    binary, problem = map(Path, sys.argv[1:])
    cases = sorted((problem / "in").glob("*.in"),
                   key=lambda p: (not p.name.startswith("example"), p.name))
    if not cases:
        sys.exit(f"Run make gen-{problem.name} first")

    rows = []
    with tempfile.TemporaryDirectory(prefix="lc-bench-", dir="/dev/shm") as tmp:
        work = Path(tmp)
        shutil.copy(binary, work / "binary")
        data, log = work / "input.in", work / "perf.json"
        for case in cases:
            shutil.copyfile(case, data)
            with data.open("rb") as source:
                run = subprocess.run(
                    ["perf", "stat", "-j", "-e", EVENTS, "-o", "perf.json", "--", "./binary"],
                    cwd=work, stdin=source, stdout=subprocess.DEVNULL,
                    stderr=subprocess.PIPE, text=True, errors="replace",
                    env={**os.environ, "LC_ALL": "C"})
            data.unlink()
            detail = log.read_text() if log.exists() else ""
            if run.returncode:
                sys.exit(f"{case.name}: perf exited {run.returncode}\n{run.stderr}{detail}")
            try:
                records = (json.loads(line) for line in detail.splitlines() if line.lstrip().startswith("{"))
                counters = {c["event"].split(":")[0].replace("cpu-cycles", "cycles"): c for c in records}
                ms, cycles, instructions, branches, misses = [number(counters.get(e)) for e in EVENTS.split(",")]
                clock = counters["task-clock"]
                ms *= {"nsec": 1e-6, "usec": 1e-3, "msec": 1, "sec": 1e3}[clock["unit"]]
            except (ValueError, KeyError, TypeError):
                sys.exit(f"{case.name}: missing/invalid task-clock\n{run.stderr}{detail}")
            ipc = instructions / cycles if instructions is not None and cycles else None
            miss_rate = 100 * misses / branches if misses is not None and branches else None
            running = min((v for c in counters.values() if (v := number(c, "pcnt-running")) is not None), default=None)
            rows.append((case.stem, ms, cycles, instructions, ipc, misses, miss_rate, running))

    # Summary rows only aggregate task-clock; other counters belong to individual cases.
    slowest = max(rows, key=lambda row: row[1])
    rows[:0] = [(f"max ({slowest[0]})", slowest[1], *[None] * 6),
                ("sum", sum(row[1] for row in rows), *[None] * 6)]
    table = [["case", clock["event"] + " (ms)", "cycles", "instructions", "IPC",
              "branch-misses", "branch-miss (%)", "running (%)"]]
    formats = ("s", ".6f", ".0f", ".0f", ".3f", ".0f", ".3f", ".1f")
    table += [["-" if v is None else format(v, f) for v, f in zip(row, formats)] for row in rows]
    widths = [max(map(len, column)) for column in zip(*table)]
    for row in table:
        print("  ".join(cell.ljust(w) if i == 0 else cell.rjust(w) for i, (cell, w) in enumerate(zip(row, widths))))


if __name__ == "__main__":
    try:
        main()
    except OSError as error:
        sys.exit(str(error))
