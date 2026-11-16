#!/usr/bin/env python3
"""Copy an agent round's matched files into src/.

Usage: merge_fixes.py FIXES_DIR... [--dry-run]

Each FIXES_DIR holds ADDR.c files (one function each, checked by the agents).
Each is re-checked with tools/check_func.py and, if it still matches, copied
to src/<binary>/<address without the leading 00>.c, the project's naming.
Files whose target already exists are reported and skipped (merge those by
hand, e.g. combined files). Then run, in the container:
    for b in main field battle viewer; do python3 tools/update_segments.py $b; done
    tools/split.sh && make && tools/check.sh
Run in the container (it calls check_func.py): tools/docker.sh python3 tools/rounds/merge_fixes.py ...
"""
import glob
import os
import shutil
import subprocess
import sys

RANGES = [("main", 0x00400000, 0x005B0000), ("field", 0x005B0000, 0x00680000),
          ("battle", 0x00680000, 0x00700000), ("viewer", 0x00700000, 0x00800000)]


def binary_of(addr):
    for b, lo, hi in RANGES:
        if lo <= addr < hi:
            return b
    raise ValueError(hex(addr))


def matches(path):
    r = subprocess.run(["python3", "tools/check_func.py", path], capture_output=True, text=True)
    lines = [l for l in (r.stdout + r.stderr).splitlines() if l.strip()]
    return bool(lines) and all(l.endswith(": OK") or l.startswith("rodata: OK") for l in lines), lines


def main():
    dry = "--dry-run" in sys.argv
    dirs = [a for a in sys.argv[1:] if not a.startswith("--")]
    merged, skipped, failed = [], [], []
    for d in dirs:
        for path in sorted(glob.glob(os.path.join(d, "*.c"))):
            name = os.path.basename(path)[:-2]
            try:
                addr = int(name, 16)
            except ValueError:
                skipped.append((path, "not an address-named file"))
                continue
            binary = binary_of(addr)
            target = f"src/{binary}/{addr & 0xFFFFFF:06X}.c"
            if os.path.exists(target):
                skipped.append((path, f"{target} exists"))
                continue
            ok, lines = matches(path)
            if not ok:
                failed.append((path, lines[:1]))
                continue
            if not dry:
                shutil.copy(path, target)
            merged.append(target)
    for t in merged:
        print("merged", t)
    for p, why in skipped:
        print("SKIPPED", p, why)
    for p, why in failed:
        print("FAILED", p, why)
    print(f"{len(merged)} merged, {len(skipped)} skipped, {len(failed)} failed" + (" (dry run)" if dry else ""))


if __name__ == "__main__":
    main()
