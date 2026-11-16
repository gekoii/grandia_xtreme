#!/usr/bin/env python3
"""Check many C files in parallel and list the ones that don't match.

Usage (in the container): check_all.py [-j N] [FILE or DIR...]
Default: every file under src/ except src/pending_inline_asm/. Prints one
line per file that fails, then a count.
"""
import glob
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor


def check(path):
    r = subprocess.run(["python3", "tools/check_func.py", path], capture_output=True, text=True)
    lines = [l for l in (r.stdout + r.stderr).splitlines() if l.strip()]
    ok = bool(lines) and all(l.endswith(": OK") or l.startswith("rodata: OK") for l in lines)
    first_bad = next((l for l in lines if not (l.endswith(": OK") or l.startswith("rodata: OK"))), "")
    return path, ok, first_bad


def main():
    args = " ".join(sys.argv[1:]).split()
    jobs = 8
    if args[:1] == ["-j"]:
        jobs, args = int(args[1]), args[2:]
    files = []
    for a in args or ["src"]:
        if os.path.isdir(a):
            files += sorted(glob.glob(os.path.join(a, "**", "*.c"), recursive=True))
        else:
            files.append(a)
    # Data-only files (bss_*.c, data/) have no functions; check_syms.py and the build check them.
    files = [f for f in files if "pending_inline_asm" not in f
             and not os.path.basename(f).startswith("bss_") and "/data/" not in f]
    bad = 0
    with ThreadPoolExecutor(jobs) as ex:
        for path, ok, why in ex.map(check, files):
            if not ok:
                bad += 1
                print(f"FAIL {path}: {why}", flush=True)
    print(f"{len(files)} files, {bad} failing")


if __name__ == "__main__":
    main()
