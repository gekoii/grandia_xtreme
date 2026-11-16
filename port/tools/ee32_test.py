#!/usr/bin/env python3
"""Tests for port/tools/ee32.py: each program in port/tests/ee32/ is
rewritten as the x86-64 build rewrites the game, compiled for x86-64
(linked below 4 GB, as the port is) and run; its output must be the one
its header comment expects.

    python port/tools/ee32_test.py [NAME...]

A test's header comment says which of its globals count as being at a PS2
offset (`placed:`, as the .bss blocks' and the linker's names are) and
what it prints (`expect:`; a | stands for a line break).
"""
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
import build  # noqa: E402
import ee32  # noqa: E402

TESTS = os.path.join(build.ROOT, "port", "tests", "ee32")
EE64 = os.path.join(build.ROOT, "port", "include", "grx_ee64.h")
FLAGS = ["-w", "-Wno-error=incompatible-pointer-types", "-Wno-error=int-conversion", "-include", EE64]


def run(name, tmp):
    src = os.path.join(TESTS, name)
    with open(src) as f:
        text = f.read()
    placed = re.search(r"\* placed:(.*)", text).group(1).split()
    expect = re.search(r"\* expect:(.*)", text).group(1).strip()
    pfile = os.path.join(tmp, name + ".placed")
    with open(pfile, "w") as f:
        f.write("\n".join(placed) + "\n")
    clang = build.direct_clang(build.find_clang())
    res = ee32.analyse((clang, "x86_64-w64-mingw32", src, ["-include", EE64, "-I", TESTS], [TESTS], None, None, pfile))
    edits, ctors, errors = ee32.merge({src: res}, lambda f: open(f, "rb").read())
    if errors:
        return "rewrite: " + "; ".join(errors)
    out_dir = os.path.join(tmp, name[:-2])
    os.makedirs(out_dir, exist_ok=True)
    for f in os.listdir(TESTS):
        p = ee32.norm(os.path.join(TESTS, f))
        if f.endswith((".c", ".h")) and (f == name or f.endswith(".h")):
            with open(p, "rb") as fi:
                data = ee32.apply(fi.read(), edits.get(p, []))
            if f == name:
                data += ee32.ctor_text(ctors.get(src, []), "t").encode()
            with open(os.path.join(out_dir, f), "wb") as fo:
                fo.write(data)
    exe = os.path.join(out_dir, "t.exe")
    cc = os.path.join(os.path.dirname(build.find_clang()), "x86_64-w64-mingw32-clang.exe")
    p = subprocess.run([cc] + FLAGS + ["-I", out_dir, os.path.join(out_dir, name), "-o", exe,
                                       "-Wl,--image-base=0x10000000", "-Wl,--disable-dynamicbase"],
                       capture_output=True, text=True)
    if p.returncode:
        return "compile: " + p.stderr[:800]
    p = subprocess.run([exe], capture_output=True, text=True)
    got = " | ".join(l.strip() for l in p.stdout.strip().splitlines())
    return None if got == expect else f"printed {got!r}, expected {expect!r}"


def main():
    names = sys.argv[1:] or sorted(f for f in os.listdir(TESTS) if f.endswith(".c"))
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        for n in names:
            err = run(n if n.endswith(".c") else n + ".c", tmp)
            print(("FAIL " if err else "ok   ") + n + (": " + err if err else ""))
            failed += err is not None
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
