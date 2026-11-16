#!/usr/bin/env python3
"""Classify every function still in assembly by what it would take to match.

Reads the split assembly (asm/<binary>/**/*.s, so only functions not yet in
C) and sorts each function into one kind:

  privileged   kernel syscall stubs and code using ei/di/mfc0/cache/sync.p
  handwritten  instructions gcc never emits (addi/add/sub/pref, $ra moved to $a0)
  mmi          128-bit integer SIMD (p*, qfsrv, mtsab, ...)
  vu0_mostly   VU0 macro-mode instructions are 25% or more of the function
  vu0_c        mostly ordinary code with a few VU0 instructions (<25%)
  c            ordinary compiler output

`$t9` is not a sign of hand-written code: gcc allocates it in big functions.
Neither are `break` (gcc's divide-by-zero trap) or the EE's pipeline-1
multiply/divide (`mult1`, `div1`, `mfhi1`, ...), which ee-gcc schedules.

Writes build/left.json (one row per function) and prints a summary.
Run from the repo root: python3 tools/rounds/classify_left.py
"""
import glob
import json
import re
import sys

LINE = re.compile(r"/\*\s*[0-9A-F]+\s+([0-9A-F]{8})\s+[0-9A-F]{8}\s*\*/\s+([a-z][a-z0-9.]*)\s*(.*)")
PRIV = {"syscall", "ei", "di", "mfc0", "mtc0", "cache", "sync.p", "eret"}
HAND = {"addi", "add", "sub", "neg", "daddi", "pref"}
MMI_EXTRA = {"qfsrv", "mtsab", "mtsah", "mfsa", "mtsa"}
VU0_EXTRA = {"lqc2", "sqc2", "vcallms", "vcallmsr"}


def is_vu0(op):
    return op in VU0_EXTRA or op.startswith(("v", "qmfc2", "qmtc2", "cfc2", "ctc2"))


def is_mmi(op):
    return op in MMI_EXTRA or (op.startswith("p") and op != "pref")


def functions():
    for path in sorted(glob.glob("asm/*/**/*.s", recursive=True)):
        binary = path.split("/")[1]
        if binary == "data" or "/data/" in path:
            continue
        name = None
        ops = []
        args = []
        for line in open(path):
            if line.startswith("glabel "):
                if name and ops:
                    yield binary, name, ops, args
                name, ops, args = line.split()[1], [], []
                continue
            m = LINE.search(line)
            if m and name:
                ops.append(m.group(2))
                args.append(m.group(3))
        if name and ops:
            yield binary, name, ops, args


def classify(ops, args):
    n = len(ops)
    s = set(ops)
    vu0 = sum(1 for o in ops if is_vu0(o))
    mmi = sum(1 for o in ops if is_mmi(o))
    if s & PRIV:
        return "privileged", vu0, mmi
    if s & HAND or any(o in ("daddu", "addu", "or") and "$ra" in a.split(",", 1)[-1] for o, a in zip(ops, args)):
        return "handwritten", vu0, mmi
    if mmi:
        return "mmi", vu0, mmi
    if vu0:
        return ("vu0_mostly" if vu0 * 4 >= n else "vu0_c"), vu0, mmi
    return "c", vu0, mmi


def main():
    rows = []
    for binary, name, ops, args in functions():
        kind, vu0, mmi = classify(ops, args)
        addr = name[5:] if name.startswith("func_") else name
        nonnop = sum(1 for o in ops if o != "nop")
        rows.append({"binary": binary, "name": name, "addr": addr, "insns": nonnop, "kind": kind,
                     "vu0": vu0, "mmi": mmi, "t9": any("$t9" in a for a in args)})
    json.dump(rows, open("build/left.json", "w"), indent=0)
    total = {}
    for r in rows:
        k = (r["kind"])
        c, i = total.get(k, (0, 0))
        total[k] = (c + 1, i + r["insns"])
    for k, (c, i) in sorted(total.items(), key=lambda x: -x[1][1]):
        print(f"{k:12} {c:5} functions {i:7} instructions")
    if "-v" in sys.argv:
        for r in sorted(rows, key=lambda r: (r["kind"], -r["insns"])):
            print(r["kind"], r["binary"], r["addr"], r["insns"], r["vu0"])


if __name__ == "__main__":
    main()
