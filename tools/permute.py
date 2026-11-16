#!/usr/bin/env python3
"""Run decomp-permuter on one near-miss C file (inside the build container).

    tools/docker.sh python3 tools/permute.py path/to/0043A508.c [--minutes 15] [-j 1]

Sets up build/permuter/<ADDR>/ the way the permuter expects:
  base.c      the C file, preprocessed (the permuter mutates this)
  target.o    the original function, assembled from splat's asm
  compile.sh  compiles a candidate through tools/compile_c.py, with the
              file's toolchain markers re-applied (preprocessing drops them);
              jump tables are included under local labels
  settings.toml

then runs the permuter until it finds a match (score 0) or time runs out.
On a match, the matching C is copied to build/permuter/<ADDR>/match.c and the
script exits 0. The permuter's output is often ugly but correct; tidy it by
hand before committing, re-checking with tools/check_func.py.
"""
import argparse
import glob
import json
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import permute_asm  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from binaries import binary_for_addr  # noqa: E402
from check_func import defined_functions  # noqa: E402
from compile_c import CFLAGS, DEFAULT_CC, MARKER_RE, CFLAGS_RE  # noqa: E402

PERMUTER = "/opt/permuter"
ASFLAGS = ["-EL", "-march=r5900", "-mabi=eabi", "-G0", "-no-pad-sections", "-I", "include"]
OBJDUMP = "mips-linux-gnu-objdump -drz -m mips:5900"


def run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def setup(c_file, workdir):
    text = open(c_file).read()
    funcs = defined_functions(text)
    if len(funcs) != 1:
        sys.exit(f"{c_file}: expected exactly one function, found {len(funcs)}")
    name, addr = funcs[0]
    seg = binary_for_addr(addr)["name"]
    asm = os.path.join(ROOT, f"build/m2c/{seg}/{addr:08X}.s")
    if not os.path.exists(asm):
        sys.exit(f"{asm} is missing (draft the function with tools/m2c_batch.py first)")
    tables = os.path.join(ROOT, f"build/m2c/{seg}/{addr:08X}.rodata.s")

    shutil.rmtree(workdir, ignore_errors=True)
    os.makedirs(workdir)
    head = text[:2000]
    markers = "".join(f"/* {k}: {v} */\n" for k, v in MARKER_RE.findall(head))
    markers += "".join(f"/* cflags: {g} */\n" for g in CFLAGS_RE.findall(head))

    # base.c: preprocessed with the file's own compiler, attributes dropped
    cc = MARKER_RE.findall(head)
    compiler = dict(cc).get("cc", DEFAULT_CC)
    pre = run([f"tools/compilers/{compiler}/bin/ee-gcc", "-E", "-P", "-D__attribute__(x)=",
               *[f for f in CFLAGS if f.startswith("-I")], c_file], cwd=ROOT)
    if pre.returncode:
        sys.exit("preprocess error:\n" + pre.stderr)
    # inline asm with operands is hidden behind placeholder calls the
    # permuter can parse (tools/permute_asm.py); compile.sh restores it
    base, asm_specs = permute_asm.hide(pre.stdout)
    with open(os.path.join(workdir, "base.c"), "w") as f:
        f.write(base)
    with open(os.path.join(workdir, "asm_specs.json"), "w") as f:
        json.dump(asm_specs, f)
    # The permuter's C parser can't read attributes, but a typedef's attribute
    # changes the type (s128 is `int __attribute__((mode(TI)))`): note each
    # one so compile.sh can put it back before compiling a candidate.
    full = run([f"tools/compilers/{compiler}/bin/ee-gcc", "-E", "-P",
                *[f for f in CFLAGS if f.startswith("-I")], c_file], cwd=ROOT)
    attrs = re.findall(r"typedef\s+([^;{}()]*?)\s+(\w+)\s+(__attribute__\s*\(\(.*?\)\))\s*;", full.stdout)
    with open(os.path.join(workdir, "typedef_attrs.json"), "w") as f:
        json.dump([[" ".join(t.split()), n, a] for t, n, a in attrs], f)

    # target.o: the original instructions, with symbolic relocations like base.o's.
    # Jump tables go in .rodata under local labels: relocations to a local
    # label are emitted against the .rodata section, the same way the compiled
    # C's are, so the two disassemblies line up.
    target_s = os.path.join(workdir, "target.s")
    with open(target_s, "w") as f:
        f.write(open(asm).read())
        if os.path.exists(tables):
            f.write("\n.section .rodata\n")
            for line in open(tables):
                m = re.match(r"dlabel (jtbl_[0-9A-F]{8})", line)
                if m:
                    f.write(f".align 4\n{m.group(1)}:\n")
                elif ".word" in line:
                    f.write(line)
    a = run(["mips-linux-gnu-as", *ASFLAGS, "-o", os.path.join(workdir, "target.o"), target_s], cwd=ROOT)
    if a.returncode:
        sys.exit("assembling the target failed:\n" + a.stderr)

    with open(os.path.join(workdir, "markers.h"), "w") as f:
        f.write(markers)
    compile_sh = os.path.join(workdir, "compile.sh")
    with open(compile_sh, "w") as f:
        f.write(f"""#!/bin/sh
# Invoked by the permuter as: compile.sh input.c -o output.o
in=$(realpath "$1"); out=$(realpath -m "$3")
tmp=$(mktemp --suffix=.c)
cat "{workdir}/markers.h" "$in" > "$tmp"
python3 - "$tmp" "{workdir}/typedef_attrs.json" <<'PY'
import json, re, sys
path, attrs = sys.argv[1], json.load(open(sys.argv[2]))
text = open(path).read()
for t, n, a in attrs:
    text = re.sub(r"typedef\\s+" + r"\\s+".join(map(re.escape, t.split())) + r"\\s+" + n + r"\\s*;",
                  f"typedef {{t}} {{n}} {{a}};", text)
open(path, "w").write(text)
PY
python3 "{ROOT}/tools/permute_asm.py" restore "$tmp" "{workdir}/asm_specs.json"
cd "{ROOT}" && python3 tools/compile_c.py "$tmp" "$out"; rc=$?
rm -f "$tmp"; exit $rc
""")
    os.chmod(compile_sh, 0o755)
    with open(os.path.join(workdir, "settings.toml"), "w") as f:
        f.write(f'func_name = "{name}"\ncompiler_type = "gcc"\nobjdump_command = "{OBJDUMP}"\n')
    return name, addr


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("c_file")
    ap.add_argument("--minutes", type=float, default=15)
    ap.add_argument("-j", type=int, default=1)
    args = ap.parse_args()

    c_file = os.path.abspath(args.c_file)
    probe = defined_functions(open(c_file).read())
    if len(probe) != 1:
        sys.exit(f"{c_file}: expected exactly one function")
    workdir = os.path.join(ROOT, f"build/permuter/{probe[0][1]:08X}")
    name, addr = setup(c_file, workdir)

    cmd = ["timeout", str(int(args.minutes * 60)), "python3", f"{PERMUTER}/permuter.py",
           workdir, "-j", str(args.j), "--stop-on-zero", "--best-only"]
    p = run(cmd, cwd=ROOT)
    outputs = sorted(glob.glob(os.path.join(workdir, "output-0-*", "source.c")))
    if outputs:
        shutil.copy(outputs[0], os.path.join(workdir, "match.c"))
        print(f"{name}: MATCH -> {workdir}/match.c")
        sys.exit(0)
    scores = sorted(int(m.group(1)) for d in glob.glob(os.path.join(workdir, "output-*"))
                    if (m := re.search(r"output-(\d+)-", d)))
    base = re.search(r"base score = (\d+)", p.stdout + p.stderr)
    if scores:
        print(f"{name}: no match in {args.minutes:g} min (best score {scores[0]})")
    else:
        # --best-only saves nothing unless a candidate beats the starting point
        print(f"{name}: no improvement in {args.minutes:g} min (base score {base.group(1) if base else '?'})")
    if p.returncode not in (0, 124):
        print(p.stdout[-500:] + p.stderr[-500:])
    sys.exit(1)


if __name__ == "__main__":
    main()
