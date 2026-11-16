#!/usr/bin/env python3
"""Move matching m2c drafts from build/m2c/<segment>/ into src/<segment>/.

Functions that sit next to each other in the binary are merged into one C
file (named after the first function's address), so a run of decompiled
functions becomes a single c subsegment. If a merged file does not compile
(usually conflicting prototypes), that run falls back to one file per
function. Run inside the build container, then tools/update_segments.py.

Usage: accept_m2c.py <segment> [function names...]
With no names, every draft with status "match" in results.json (and
results_jtbl.json, for switch functions) is taken.
"""
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(__file__))
from m2c_batch import COMPILER, CFLAGS, SEGMENTS, SYMBOL_ADDRS, already_decompiled, find_functions  # noqa: E402

HEADER = '#include "common.h"\n#include "m2c_macros.h"\n'
DEF_RE = re.compile(r"^[A-Za-z_].*\b(func_[0-9A-F]{8})\(.*\)\s*\{\s*$")
DECL_NAME_RE = re.compile(r"\b((?:func|D)_[0-9A-F]{6,8}\w*)\b")


def split_draft(path):
    """Return (declaration lines, function definition text) of an m2c draft."""
    lines = open(path).read().splitlines()
    for i, line in enumerate(lines):
        if DEF_RE.match(line):
            decls = [l for l in lines[:i] if l.strip() and not l.startswith("#include")]
            return decls, "\n".join(lines[i:]).rstrip() + "\n"
    raise ValueError(f"no function definition in {path}")


def compiles(path):
    return subprocess.run([COMPILER, *CFLAGS, "-S", "-o", "/dev/null", path],
                          capture_output=True).returncode == 0


def write_file(path, parts):
    """parts: list of (decls, body). Declarations of functions defined in this
    file are dropped; the rest are de-duplicated in order."""
    defined = {re.search(r"func_[0-9A-F]{8}", body.splitlines()[0]).group(0) for _, body in parts}
    seen, decls = set(), []
    for d, _ in parts:
        for line in d:
            names = DECL_NAME_RE.findall(line)
            if names and names[0] in defined:
                continue
            if line not in seen:
                seen.add(line)
                decls.append(line)
    with open(path, "w") as f:
        f.write(HEADER + "\n")
        if decls:
            f.write("\n".join(decls) + "\n\n")
        f.write("\n".join(body for _, body in parts))


def main():
    segment = sys.argv[1]
    names = set(sys.argv[2:])
    asm_glob, _, _ = SEGMENTS[segment]
    workdir = f"build/m2c/{segment}"
    results = json.load(open(os.path.join(workdir, "results.json")))
    if os.path.exists(os.path.join(workdir, "results_jtbl.json")):
        results += json.load(open(os.path.join(workdir, "results_jtbl.json")))
    chosen = {r["name"] for r in results if (r["name"] in names if names else r["status"] == "match")}

    # Functions already in src/ are not in the asm any more, but they still
    # separate runs: drafts on either side of one must not be merged.
    order = find_functions(asm_glob)
    order += [{"name": n, "addr": int(n[5:], 16) if n.startswith("func_") else SYMBOL_ADDRS[n]}
              for n in already_decompiled(segment)]
    order.sort(key=lambda f: f["addr"])
    runs, current = [], []
    for f in order:
        if f["name"] in chosen:
            current.append(f)
        elif current:
            runs.append(current)
            current = []
    if current:
        runs.append(current)

    os.makedirs(f"src/{segment}", exist_ok=True)
    files = 0
    for run in runs:
        parts = [split_draft(os.path.join(workdir, f"{f['addr']:08X}.c")) for f in run]
        path = f"src/{segment}/{run[0]['addr']:06X}.c"
        write_file(path, parts)
        files += 1
        if len(run) > 1 and not compiles(path):
            os.remove(path)
            files -= 1
            for f, part in zip(run, parts):
                write_file(f"src/{segment}/{f['addr']:06X}.c", [part])
                files += 1
    print(f"accepted {len(chosen)} functions into {files} files")


if __name__ == "__main__":
    main()
