#!/usr/bin/env python3
"""Expand toy, ACL and relative project headers; keep system includes and comments."""
import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LIBS = {"toy": ROOT / "include", "atcoder": ROOT / "ac-library"}
INCLUDE = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]\s*(?:(//.*|/\*.*\*/)\s*)?$')


def resolve(name, delimiter, current):
    candidates = [current.parent / name] if delimiter == '"' else []
    library = LIBS.get(name.split('/')[0])
    if library:
        candidates.append(library / name)
    for path in candidates:
        path = path.resolve()
        if path.is_relative_to(ROOT) and path.is_file():
            return path
    if library:
        raise FileNotFoundError(name)


def expand(path, seen):
    path = path.resolve()
    if path in seen:
        return []
    seen.add(path)
    result = [f"// BEGIN bundled: {path.relative_to(ROOT)}"]
    for line in path.read_text().splitlines():
        if line.strip() == "#pragma once":
            continue
        match = INCLUDE.match(line)
        if match:
            nested = resolve(match[2], match[1], path)
            if nested:
                if match[3]:
                    result.append(match[3])
                result.extend(expand(nested, seen))
                continue
        result.append(line)
    result.append(f"// END bundled: {path.relative_to(ROOT)}")
    return result


def save(path, text):
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + '.part')
    temporary.write_text(text)
    temporary.replace(path)


def make_path(path):
    return str(path).replace('\\', '\\\\').replace('$', '$$').replace('#', '\\#').replace(' ', '\\ ')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', type=Path)
    parser.add_argument('-o', '--output', type=Path, required=True)
    parser.add_argument('--depfile', type=Path)
    args = parser.parse_args()
    seen = set()
    content = '\n'.join(expand(ROOT / args.source, seen)) + '\n'
    if args.depfile:
        deps = [make_path(path.relative_to(ROOT)) for path in sorted(seen)]
        save(ROOT / args.depfile, make_path(args.output) + ': ' + ' '.join(deps) + '\n'
             + ''.join(path + ':\n' for path in deps))
    save(ROOT / args.output, content)


if __name__ == '__main__':
    main()
