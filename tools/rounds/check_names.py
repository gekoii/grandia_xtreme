#!/usr/bin/env python3
"""Cross-check naming proposals before applying them with tools/rename.py.

Reads every names file given (default: build/names/libs_main.txt and
build/names/proposals/*.txt) and reports
  - a new name proposed for two different symbols,
  - a symbol given two different names,
  - a new name that is already a symbol name in config/symbol_addrs*.txt.
Lines that are commented out (#) are ignored.

Run from the repo root: python3 tools/rounds/check_names.py [FILE...]
"""
import collections
import glob
import re
import sys


def load(paths):
    rows = []
    for path in paths:
        for n, line in enumerate(open(path), 1):
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            body = line.split("//", 1)[0].split()
            if len(body) == 2:
                rows.append((body[0], body[1], f"{path}:{n}"))
    return rows


def main():
    paths = sys.argv[1:] or ["build/names/libs_main.txt"] + sorted(glob.glob("build/names/proposals/*.txt"))
    rows = load(paths)
    existing = set()
    for p in glob.glob("config/symbol_addrs*.txt"):
        existing.update(re.findall(r"^(\w+)\s*=", open(p).read(), re.M))
    by_new = collections.defaultdict(list)
    by_old = collections.defaultdict(list)
    for old, new, where in rows:
        by_new[new].append((old, where))
        by_old[old].append((new, where))
    problems = 0
    for new, olds in sorted(by_new.items()):
        if len({o for o, _ in olds}) > 1:
            problems += 1
            print(f"name {new} given to: " + ", ".join(f"{o} ({w})" for o, w in olds))
    for old, news in sorted(by_old.items()):
        if len({n for n, _ in news}) > 1:
            problems += 1
            print(f"symbol {old} named: " + ", ".join(f"{n} ({w})" for n, w in news))
    for old, new, where in rows:
        if new in existing and old != new:
            problems += 1
            print(f"{where}: {new} is already a symbol in config/symbol_addrs*.txt")
    print(f"{len(rows)} names in {len(paths)} files, {problems} problems")
    sys.exit(1 if problems else 0)


if __name__ == "__main__":
    main()
