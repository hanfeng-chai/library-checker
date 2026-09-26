#!/usr/bin/env python3
"""Fetch the 5 fastest distinct-user AC submissions per problem; C/C++/Rust only.

Writes ``submissions/<category>/<problem>/<user>.cpp|.c|.rs`` and ``<user>.json``
(the OJ response minus ``source``, which is already the sibling file;
``overview.time``/``memory`` are per-case maxima, not sums).  Existing files are
skipped, so an interrupted run resumes.
"""
import json, re, sys, threading, time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import requests

API, ROOT = "https://v3.api.judge.yosupo.jp", Path(__file__).resolve().parents[1]
EXT = {"c": ".c", "cpp": ".cpp", "cpp17": ".cpp", "cpp20": ".cpp", "rust": ".rs"}
LOCK, NEXT = threading.Lock(), [0.0]

def get(session, path, params=None):
    """GET, holding every request start 0.5 s apart."""
    with LOCK:
        time.sleep(max(0.0, NEXT[0] - time.monotonic()))
        NEXT[0] = time.monotonic() + 0.5
    response = session.get(API + path, params=params, timeout=30)
    response.raise_for_status()
    return response.json()

def run(category, problem, limit=5):
    """Write the fastest ``limit`` distinct users of one problem."""
    folder, users, found, written = ROOT / "submissions" / category / problem, set(), 0, 0
    try:
        session = requests.Session()
        for skip in range(0, 1000, 100):
            query = {"problem": problem, "status": "AC", "order": "+time", "skip": skip, "limit": 100}
            for row in get(session, "/submissions", query).get("submissions", []):
                user = row.get("user_name") or "Anonymous"
                if user in users or row["lang"] not in EXT:
                    continue
                users.add(user)
                found += 1
                out = folder / f"{re.sub(r'[^A-Za-z0-9_-]', '_', user)}.json"
                if not out.exists():
                    detail = get(session, f"/submissions/{row['id']}")
                    folder.mkdir(parents=True, exist_ok=True)
                    out.with_suffix(EXT[row["lang"]]).write_text(detail.pop("source", ""), encoding="utf-8")
                    out.write_text(json.dumps(detail, indent=2, sort_keys=True))
                    written += 1
                if found == limit:
                    return f"{category}/{problem}: {written} new"
    except Exception as error:
        return f"{category}/{problem}: FAILED {error}"
    return f"{category}/{problem}: {written} new"

if __name__ == "__main__":
    pair = lambda p: (p.parent.parent.name, p.parent.name)
    todo = [pair(p) for p in sorted((ROOT / "problems").glob("*/*/info.toml"))
            if pair(p)[0] not in {"test", "common", "docs"}]
    todo = [item for item in todo if not sys.argv[1:] or item[1] in sys.argv[1:]]
    with ThreadPoolExecutor(max_workers=4) as pool:
        for index, line in enumerate(pool.map(lambda item: run(*item), todo), 1):
            print(f"[{index}/{len(todo)}] {line}", flush=True)
