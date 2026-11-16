#!/usr/bin/env python3
"""Switch C files over to the generated prototype header, one file at a time.

For each file in src/<binary>/: drop its own one-line prototypes of functions
that include/protos/<binary>.h declares, add `#include "protos/<binary>.h"`,
and re-check the file with tools/check_func.py. The change is kept only if
every function in the file (and its .rodata) still matches; otherwise the file
is left as it was. Files that still need a different prototype (a caller whose
code depends on a deviant declaration) stay on their local ones.

Run inside the build container: tools/docker.sh python3 tools/apply_protos.py <binary> [-j N]
Prints a summary and writes the list of kept and rejected files to
build/protos_<binary>.txt.
"""
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from binaries import BINARIES  # noqa: E402

PROTO_LINE = re.compile(r"^(?!static\b)(?:extern\s+)?[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{}()]*\)\s*;[ \t]*(/\*.*\*/)?[ \t]*\n", re.M)


def header_names(binary):
    names = set()
    for b in {binary, "main"}:
        for line in open(f"include/protos/{b}.h"):
            m = re.search(r"\b([A-Za-z_]\w*)\(", line)
            if m and line.rstrip().endswith(";"):
                names.add(m.group(1))
    return names


def convert(text, binary, names):
    """The file's text using the header, or None if there's nothing to do."""
    if f'"protos/{binary}.h"' in text or '#include "common.h"' not in text:
        return None
    removed = []

    def drop(m):
        if m.group(1) in names:
            removed.append(m.group(1))
            return ""
        return m.group(0)

    new = PROTO_LINE.sub(drop, text)
    if not removed:
        return None
    return new.replace('#include "common.h"\n', f'#include "common.h"\n#include "protos/{binary}.h"\n', 1)


def matches(path):
    r = subprocess.run(["python3", "tools/check_func.py", path], capture_output=True, text=True)
    out = r.stdout + r.stderr
    lines = [l for l in out.splitlines() if l.strip()]
    return bool(lines) and all(l.endswith(": OK") or l.startswith("rodata: OK") for l in lines)


def try_file(path, binary, names):
    text = open(path).read()
    new = convert(text, binary, names)
    if new is None:
        return path, "skip"
    tmp = os.path.join("build", "protos_tmp", binary, os.path.basename(path))
    os.makedirs(os.path.dirname(tmp), exist_ok=True)
    with open(tmp, "w") as f:
        f.write(new)
    if matches(tmp):
        with open(path, "w") as f:
            f.write(new)
        return path, "kept"
    return path, "rejected"


def main():
    binary = sys.argv[1]
    jobs = int(sys.argv[sys.argv.index("-j") + 1]) if "-j" in sys.argv else 8
    names = header_names(binary)
    src = BINARIES[binary]["src"]
    files = sorted(os.path.join(src, f) for f in os.listdir(src) if f.endswith(".c"))
    with ThreadPoolExecutor(jobs) as ex:
        results = list(ex.map(lambda p: try_file(p, binary, names), files))
    counts = {}
    for _, status in results:
        counts[status] = counts.get(status, 0) + 1
    with open(f"build/protos_{binary}.txt", "w") as f:
        for path, status in results:
            f.write(f"{status} {path}\n")
    print(binary, counts)


if __name__ == "__main__":
    main()
