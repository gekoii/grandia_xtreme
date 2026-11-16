#!/usr/bin/env python3
"""Remove call-site casts that the match doesn't need.

tools/cast_calls.py casts every call whose local prototype differed from the
header, but often only one of those differences changes the generated code.
For each file and each function called through a cast, this drops that
function's casts (calling it through the header's prototype instead),
re-checks the file, and keeps the removal only if the file still matches.
It also drops casts that become redundant after a definition changes.

Run in the container: tools/docker.sh python3 tools/drop_casts.py [file ...]
(default: every file under src/ with a cast).
"""
import glob
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

CAST = re.compile(r"\(\(([^()]*\(\*\)\([^()]*\))\)([A-Za-z_]\w*)\)\(")


def matches(path):
    r = subprocess.run(["python3", "tools/check_func.py", path], capture_output=True, text=True)
    lines = [l for l in (r.stdout + r.stderr).splitlines() if l.strip()]
    return bool(lines) and all(l.endswith(": OK") or l.startswith("rodata: OK") for l in lines)


def minimize(path):
    text = open(path).read()
    names = sorted(set(n for _, n in CAST.findall(text)))
    dropped = 0
    tmp = os.path.join("build", "dropcasts_tmp", path.replace("/", "_"))
    os.makedirs(os.path.dirname(tmp), exist_ok=True)
    for name in names:
        trial = re.sub(r"\(\([^()]*\(\*\)\([^()]*\)\)" + name + r"\)\(", name + "(", text)
        with open(tmp, "w") as f:
            f.write(trial)
        if matches(tmp):
            text = trial
            dropped += 1
    if dropped:
        with open(path, "w") as f:
            f.write(text)
    return path, len(names), dropped


def main():
    files = sys.argv[1:] or [p for p in sorted(glob.glob("src/*/*.c")) if CAST.search(open(p).read())]
    with ThreadPoolExecutor(8) as ex:
        results = list(ex.map(minimize, files))
    total = sum(n for _, n, _ in results)
    dropped = sum(d for _, _, d in results)
    print(f"{len(files)} files: {dropped} of {total} cast functions removed")


if __name__ == "__main__":
    main()
