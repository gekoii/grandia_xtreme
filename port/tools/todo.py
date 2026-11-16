#!/usr/bin/env python3
"""List the functions the port still stubs (build/port/stubs.c, written by
port/tools/build.py) with where their assembly is, so they can be written
in C under port/game/<binary>/.

    python port/tools/todo.py            # TSV: name binary addr insns kind asm best

kind: "platform" for kernel calls and hardware handlers (the platform layer
implements those, port/README.md), "vu0"/"mmi" for hand-written vector code,
"code" for the rest. best: a near miss's best C attempt from the
decompilation rounds, if one was kept.
"""

import glob
import os
import re
import sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), "..", ".."))

# Kernel syscalls, interrupt/DMA handler plumbing, cache control, the
# kernel's own internals: the platform layer's job, not C translations
PLATFORM = re.compile(
    r"^(_?i?(Add|Remove|Enable|Disable)(Dmac|Intc)(Handler2?)?|_?[A-Z]\w*Thread\w*|\w*Sema|"
    r"_?(Sync|Invalid|Flush)\w*Cache|FlushCache|SetAlarm|SetGsCrt|SetVSyncFlag|Gs[PG]\w*IMR|"
    r"[DE]Intr|Deci2Call|Exit|__\w+|_sce\w+|sce\w+|_i?\w*(Cache|Dmac|Intc)|\w+_VRAM|GameMain)$")


def stubs():
    # A build made with build.py --out DIR: todo.py DIR
    out = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "build", "port")
    path = os.path.join(out, "stubs.c")
    if not os.path.exists(path):
        sys.exit(f"{path} not found: run port/tools/build.py first")
    with open(path) as f:
        return re.findall(r"^void (\w+)\(void\)", f.read(), re.M)


def asm_functions():
    """{name: (binary, asm path, address, instruction count, hand-written)}"""
    out = {}
    for path in glob.glob(os.path.join(ROOT, "asm", "**", "*.s"), recursive=True):
        if os.sep + "data" + os.sep in path:
            continue
        binary = os.path.relpath(path, os.path.join(ROOT, "asm")).split(os.sep)[0]
        cur = None
        with open(path, encoding="latin-1") as f:
            for line in f:
                m = re.match(r"glabel (\w+)", line)
                if m:
                    cur = [binary, os.path.relpath(path, ROOT).replace("\\", "/"), None, 0, ""]
                    out[m.group(1)] = cur
                    continue
                if cur is None:
                    continue
                m = re.match(r"\s*/\* \w+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(\S+)(.*)", line)
                if m:
                    cur[2] = cur[2] or m.group(1)
                    cur[3] += 1
                    ins = m.group(2)
                    if ins.startswith("v") or "c2" in ins or ins in ("vcallms", "vcallmsr"):
                        cur[4] = cur[4] or "vu0"
                    elif ins.startswith(("p", "q")) and ins not in ("pref",):
                        cur[4] = cur[4] or "mmi"
                if line.startswith("endlabel"):
                    cur = None
    return out


def main():
    funcs = asm_functions()
    best = {os.path.basename(p)[:-2].replace("best_", ""): p
            for p in glob.glob(os.path.join(ROOT, "build", "agents", "best", "*.c"))
            + glob.glob(os.path.join(ROOT, "build", "agents", "**", "best_*.c"), recursive=True)}
    print("name\tbinary\taddr\tinsns\tkind\tasm\tbest")
    for name in stubs():
        info = funcs.get(name)
        if not info:
            print(f"{name}\t?\t?\t0\t{'platform' if PLATFORM.match(name) else 'missing'}\t\t")
            continue
        binary, path, addr, n, vec = info
        kind = "platform" if PLATFORM.match(name) else (vec or "code")
        b = best.get(addr or "", "")
        print(f"{name}\t{binary}\t{addr}\t{n}\t{kind}\t{path}\t{os.path.relpath(b, ROOT).replace(os.sep, '/') if b else ''}")


if __name__ == "__main__":
    main()
