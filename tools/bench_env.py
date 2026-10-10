#!/usr/bin/env python3
"""Read-only benchmark environment check; exit 1 for failed or unknown settings."""
import os
from pathlib import Path

CPU = Path("/sys/devices/system/cpu")
KERNEL = Path("/proc/sys/kernel")


def read(path):
    try:
        return path.read_text().strip()
    except OSError:
        return None


def main():
    cpus = sorted(os.sched_getaffinity(0))
    policies = [CPU / f"cpu{cpu}/cpufreq" for cpu in cpus]
    print(f"cpus: {','.join(map(str, cpus))} (inherited affinity; no pinning)")
    checks = [("governor", policy / "scaling_governor", "performance") for policy in policies]
    boost = [(CPU / "intel_pstate/no_turbo", "1"),
             (CPU / "cpufreq/boost", "0")] + [(policy / "boost", "0") for policy in policies]
    boost = [(path, expected) for path, expected in boost if path.exists()]
    checks += [("Turbo/boost", path, expected) for path, expected in boost]
    checks += [("ASLR", KERNEL / "randomize_va_space", "0"),
               ("NMI watchdog", KERNEL / "nmi_watchdog", "0"),
               ("SMT", CPU / "smt/active", "0")]
    ready = bool(boost)
    if not boost:
        print("[UNKNOWN] Turbo/boost: no supported control found")
    for name, path, expected in checks:
        value = read(path)
        status = "UNKNOWN" if value is None else "OK" if value == expected else "FAIL"
        print(f"[{status}] {name}: {value!r}, expected {expected!r} ({path})")
        ready &= status == "OK"
    print("Configuration checks only; repeatability still needs measurement.")
    return 0 if ready else 1


if __name__ == "__main__":
    raise SystemExit(main())
