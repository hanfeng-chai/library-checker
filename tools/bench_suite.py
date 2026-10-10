#!/usr/bin/env python3
"""Rebuild/check completed problems; stage two quiet passes; publish per-case minima."""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import resource
import shutil
import subprocess
import tempfile
import time

from bench import EVENTS, counters, table

ROOT = Path(__file__).resolve().parent.parent


def digest(path):
    with path.open("rb") as f:
        return hashlib.file_digest(f, "sha256").hexdigest()


def save(path, value):
    path.with_suffix(".tmp").write_text(json.dumps(value, indent=2) + "\n")
    path.with_suffix(".tmp").replace(path)


def stack():
    hard = resource.getrlimit(resource.RLIMIT_STACK)[1]
    resource.setrlimit(resource.RLIMIT_STACK, (hard, hard))


def files():
    paths = {ROOT / f for f in ("Makefile", "cpp_flags.txt", "cxx_flags.txt", "rust_flags.txt")}
    for folder in ("include/toy", "ac-library/atcoder", "tools"):
        paths.update(p for p in (ROOT / folder).rglob("*") if p.is_file() and "__pycache__" not in p.parts)
    for main in (ROOT / "src").glob("*/*/main.cxx"):
        problem = main.parent.relative_to(ROOT / "src")
        for folder in (main.parent, ROOT / "submissions" / problem):
            paths.update(p for p in folder.glob("*") if p.suffix in (".cxx", ".cpp", ".rs", ".md", ".json"))
        folder = ROOT / "problems" / problem
        paths.update([folder / "info.toml", folder / "checker"])
        paths.update(p for part in ("in", "out") for p in (folder / part).iterdir() if p.is_file())
    print("\n".join(sorted(str(p.relative_to(ROOT)) for p in paths)))


def prepare(base, jobs):
    base.mkdir(parents=True, exist_ok=True)
    if (base / "manifest.json").exists():
        raise RuntimeError("Use a new stage directory for a new batch")
    manifest = {}
    for main in sorted((ROOT / "src").glob("*/*/main.cxx")):
        problem = str(main.parent.relative_to(ROOT / "src"))
        owned = sorted(main.parent.glob("*.cxx"))
        refs = sorted(p for p in (ROOT / "submissions" / problem).glob("*") if p.suffix in (".cpp", ".rs"))
        assert not refs if problem == "sample/integer_checksum" else len(refs) >= 5, problem
        sources = refs + owned
        assert len({p.stem for p in sources}) == len(sources), problem
        cases = sorted((ROOT / "problems" / problem / "in").glob("*.in"),
                       key=lambda p: (not p.name.startswith("example"), p.name))
        assert cases, f"Run make gen-{main.parent.name} first"
        manifest[problem] = dict(solutions={p.stem: str(p.relative_to(ROOT)) for p in sources},
                                 cases=[p.stem for p in cases])
    targets = [f"build/{p}/{s}" for p, item in manifest.items() for s in item["solutions"]]
    stack()
    for step, goals in [("compile", ["-B", *targets]),
                        ("check", [f"check-{Path(p).name}" for p in manifest])]:
        if step == "check":
            for problem in manifest:
                shutil.rmtree(ROOT / "check" / problem, ignore_errors=True)
        print(f"{step}: {len(targets)} binaries, {len(manifest)} problems", flush=True)
        with (base / f"{step}.log").open("w") as log:
            result = subprocess.run(["make", f"-j{jobs}", *goals], cwd=ROOT, stdout=log, stderr=log)
        if result.returncode:
            raise RuntimeError((base / f"{step}.log").read_text()[-8000:])
    hashes = {}
    for problem, item in manifest.items():
        for name, source in item["solutions"].items():
            source = ROOT / source
            for original, relative in [(ROOT / "build" / problem / name, f"bin/{problem}/{name}"),
                                       (source, f"sources/{source.relative_to(ROOT)}")]:
                target = base / relative
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(original, target)
                hashes[relative] = digest(target)
        for case in item["cases"]:
            relative = f"cases/{problem}/{case}.in"
            target = base / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            os.link(ROOT / "problems" / problem / "in" / f"{case}.in", target)
            hashes[relative] = digest(target)
    for source in [*(ROOT / "include/toy").glob("*.h"), *(ROOT / "include/toy").glob("*.hpp"),
                   *[p for p in (ROOT / "ac-library/atcoder").rglob("*") if p.is_file()],
                   *[ROOT / f for f in ("cpp_flags.txt", "cxx_flags.txt", "rust_flags.txt", "Makefile")]]:
        target = base / "sources" / source.relative_to(ROOT)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        hashes[str(target.relative_to(base))] = digest(target)
    for name in ("bench_suite.py", "bench.py", "bench_env.py"):
        shutil.copy2(ROOT / "tools" / name, base / name)
        hashes[name] = digest(base / name)
    compiler = {c: subprocess.check_output([c, "--version"], text=True).splitlines()[0]
                for c in ("g++", "rustc")}
    commit = ((ROOT / ".source-commit").read_text().strip() if (ROOT / ".source-commit").exists()
              else subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=ROOT, text=True).strip())
    save(base / "manifest.json", dict(problems=manifest, sha256=hashes, compiler=compiler,
                                     compile_host=platform.node(), compile_libc=os.confstr("CS_GNU_LIBC_VERSION"),
                                     created=datetime.datetime.now().astimezone().isoformat(),
                                     commit=commit))
    print(f"Prepared {base}", flush=True)


def inputs(base):
    manifest = json.loads((base / "manifest.json").read_text())
    for name, expected in manifest["sha256"].items():
        if not name.startswith("cases/"):
            continue
        case = Path(name.removeprefix("cases/"))
        source = ROOT / "problems" / case.parent / "in" / case.name
        assert digest(source) == expected, name
        target = base / name
        target.parent.mkdir(parents=True, exist_ok=True)
        if not target.exists():
            os.link(source, target)
    print("Inputs match the checked build host", flush=True)


def run(base):
    if (base / "result.json").exists() or (base / "round-1.json").exists():
        raise RuntimeError("This batch already has results; use a fresh directory")
    stack()
    subprocess.run(["python3", str(base / "bench_env.py")], check=True)
    manifest = json.loads((base / "manifest.json").read_text())
    for name, expected in manifest["sha256"].items():
        assert digest(base / name) == expected, name
    environment = dict(host=platform.node(), kernel=platform.release(),
                       affinity=sorted(os.sched_getaffinity(0)), stack=resource.getrlimit(resource.RLIMIT_STACK),
                       cpu=next(s.split(":", 1)[1].strip() for s in Path("/proc/cpuinfo").read_text().splitlines()
                                if s.startswith("model name")),
                       started=datetime.datetime.now().astimezone().isoformat())
    rows = []
    with tempfile.TemporaryDirectory(prefix="lc-suite-", dir="/dev/shm") as tmp:
        work = Path(tmp)
        for problem, item in manifest["problems"].items():
            for name in item["solutions"]:
                target = work / "bin" / problem / name / "binary"
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(base / "bin" / problem / name, target)
        data, log = work / "input.in", work / "perf.json"
        os.sync()
        time.sleep(15)
        # Two complete passes. No reporting, compilation or other remote commands here.
        for repeat in range(2):
            start = len(rows)
            for problem, item in manifest["problems"].items():
                for case in item["cases"]:
                    shutil.copyfile(base / "cases" / problem / f"{case}.in", data)
                    for name in item["solutions"]:
                        binary = work / "bin" / problem / name / "binary"
                        with data.open("rb") as source:
                            result = subprocess.run(["perf", "stat", "-j", "-e", EVENTS, "-o", str(log),
                                                     "--", "./binary"], cwd=binary.parent, stdin=source,
                                                    stdout=subprocess.DEVNULL, stderr=subprocess.PIPE,
                                                    env={**os.environ, "LC_ALL": "C"})
                        detail = log.read_text()
                        if result.returncode:
                            raise RuntimeError(f"{problem}/{name}/{case}: exit {result.returncode}\n"
                                               + result.stderr.decode(errors="replace") + detail)
                        counters(detail)
                        rows.append(dict(problem=problem, binary=name, case=case, round=repeat + 1, perf=detail))
                    data.unlink()
            # Checkpoints contain a completed pass; partial batches cannot be published.
            save(base / f"round-{repeat + 1}.json", rows[start:])
            if repeat == 0:
                os.sync()
                time.sleep(15)
    environment["finished"] = datetime.datetime.now().astimezone().isoformat()
    save(base / "result.json", dict(manifest=manifest, environment=environment, results=rows))
    print(f"Done: {len(rows)} measurements", flush=True)


def selected(result):
    observed = {}
    for row in result["results"]:
        key = row["problem"], row["binary"], row["case"]
        rounds = observed.setdefault(key, {})
        assert row["round"] not in rounds, key
        rounds[row["round"]] = row
    expected = {(p, s, c) for p, item in result["manifest"]["problems"].items()
                for s in item["solutions"] for c in item["cases"]}
    assert set(observed) == expected, "Incomplete or unexpected measurements"
    assert all(set(rounds) == {1, 2} for rounds in observed.values()), "Both full passes are required"
    return {key: min(rounds.values(), key=lambda r: counters(r["perf"])[0]) for key, rounds in observed.items()}


def report(base):
    result = json.loads((base / "result.json").read_text())
    best = selected(result)
    for name, expected in result["manifest"]["sha256"].items():
        if name.startswith("sources/"):
            assert digest(ROOT / name.removeprefix("sources/")) == expected, name
        elif name.startswith("cases/"):
            case = Path(name.removeprefix("cases/"))
            assert digest(ROOT / "problems" / case.parent / "in" / case.name) == expected, name
    summary = {}
    for problem, item in result["manifest"]["problems"].items():
        sections, stats = [], {}
        for name, source in item["solutions"].items():
            assert digest(ROOT / "build" / problem / name) == result["manifest"]["sha256"][f"bin/{problem}/{name}"], name
            assert digest(ROOT / source) == result["manifest"]["sha256"][f"sources/{source}"], source
            rows = [(case, *counters(best[problem, name, case]["perf"])) for case in item["cases"]]
            stats[name] = dict(max=max(r[1] for r in rows), sum=sum(r[1] for r in rows))
            measured = result.get("provenance", {}).get(problem, {}).get(name)
            compiler = item.get("compilers", {}).get(name)
            sections.append(f"\n{source}" + (f" [{name}]" if name != Path(source).stem else "") + "\nSHA-256: "
                            + result["manifest"]["sha256"][f"bin/{problem}/{name}"] + "\n"
                            + (f"Compiler: {compiler}\n" if compiler else "")
                            + (f"Measured: {measured}\n" if measured else "") + table(rows))
        refs = [s for s, source in item["solutions"].items() if source.startswith("submissions/")]
        winners = {m: min(refs, key=lambda s: stats[s][m]) for m in ("max", "sum")} if refs else {}
        date = result["environment"]["finished"][:10]
        meta = f"{date} {result['environment']['host']} / {result['environment']['cpu']}\n"
        meta += "Two complete passes per binary; per case minimum task-clock; all counters from that same run.\n"
        if result.get("provenance"):
            meta += "Unchanged binaries reuse their recorded passes; each section identifies its measurement batch.\n"
        meta += f"Cases: {len(item['cases'])}, including examples. Units: ms. No CPU pinning.\n"
        meta += f"Compiler: {result['manifest']['compiler']['g++']}; {result['manifest']['compiler']['rustc']}\n"
        meta += f"Build host: {result['manifest'].get('compile_host', '?')}; "
        meta += f"{result['manifest'].get('compile_libc', '?')}\n"
        meta += f"Environment: Linux {result['environment']['kernel']}; affinity {result['environment']['affinity']}; "
        meta += f"stack {result['environment']['stack'][0]} bytes; bench_env OK.\n"
        for flag in ("cxx_flags.txt", "cpp_flags.txt", "rust_flags.txt"):
            meta += flag + ": " + " ".join((base / "sources" / flag).read_text().split()) + "\n"
        overview = "\nsolution                                max (ms)       sum (ms)\n"
        overview += "".join(f"{name:<36} {v['max']:14.6f} {v['sum']:14.6f}\n" for name, v in stats.items())
        (ROOT / "src" / problem / "bench.txt").write_text(meta + overview + "".join(sections))
        top = ["<!-- benchmark-summary -->", f"{date} Lenovo，每个程序两轮完整测例，逐例取较小 task-clock；单位 ms，包含样例。"]
        if refs:
            top.append(f"参考最佳（{len(refs)} 份）：" + ", ".join(f"{m} {stats[winners[m]][m]:.3f} ({winners[m]})" for m in ("max", "sum")) + "。")
        else:
            top.append("自建题，无 OJ 参考；以下数值仅比较本题的自有实现。")
        top.append("")
        owned = sorted((s for s, source in item["solutions"].items() if source.startswith("src/")),
                       key=lambda s: ({"main": 0, "naive": 1}.get(s, 2), s))
        for name in owned:
            values = []
            for m in ("max", "sum"):
                value = f"{m}: {stats[name][m]:.3f} ms"
                if refs:
                    value += f" ({100 * (stats[name][m] / stats[winners[m]][m] - 1):+.2f}%)"
                values.append(value)
            label = item.get("labels", {}).get(name, f"{name}.cxx")
            top.append(f"- `{label}` " + ", ".join(values))
        top += ["", "明细见 [bench.txt](bench.txt)，方法见 [benchmark.md](../../../tools/benchmark.md)。",
                "<!-- /benchmark-summary -->", ""]
        tutorial = ROOT / "src" / problem / "tutorial.md"
        text = tutorial.read_text()
        text = re.sub(r"<!-- benchmark-summary -->.*?<!-- /benchmark-summary -->\n*", "", text, flags=re.S)
        title, rest = text.split("\n", 1)
        tutorial.write_text(title + "\n\n" + "\n".join(top) + rest.lstrip("\n"))
        summary[problem] = dict(stats=stats, best_reference=winners,
                                main_wins=all(stats["main"][m] < stats[winners[m]][m] for m in winners) if refs else None)
    save(base / "summary.json", summary)
    failures = [p for p, s in summary.items() if s["main_wins"] is False]
    print(f"Reported {len(summary)} problems; main fails max/sum: {failures}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("files", "prepare", "inputs", "run", "report"))
    parser.add_argument("stage", type=Path, nargs="?")
    parser.add_argument("-j", "--jobs", type=int, default=4)
    args = parser.parse_args()
    if args.action == "files":
        files()
        return
    if args.stage is None:
        parser.error("stage is required for prepare/run/report")
    if args.action == "prepare":
        prepare(args.stage.resolve(), args.jobs)
    else:
        {"inputs": inputs, "run": run, "report": report}[args.action](args.stage.resolve())


if __name__ == "__main__":
    main()
