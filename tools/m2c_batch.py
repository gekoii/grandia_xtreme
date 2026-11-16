#!/usr/bin/env python3
"""Run m2c over many functions and check which drafts already match.

For each candidate function in a splat asm file this:
  1. cuts the function out into its own .s file,
  2. runs m2c on it to get first-draft C,
  3. compiles the draft with ee-gcc (same flags and fix-ups as the Makefile),
  4. compares the result with the original bytes, resolving each relocation
     (call targets, %hi/%lo halves) to the address its symbol name gives.

Drafts go to build/m2c/<segment>/<address>.c and a summary to
build/m2c/<segment>/results.json. Run inside the build container:

    tools/docker.sh python3 tools/m2c_batch.py main --max-insns 24

A "match" here is checked word for word, but only the real build
(tools/split.sh + make) links everything and compares the final binaries.
"""
import argparse
import glob
import json
import os
import re
import struct
import subprocess
from multiprocessing import Pool
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from compile_c import compile_c  # noqa: E402

SEGMENTS = {
    # name: (asm glob, rom file, vram of rom offset 0)
    "main": ("asm/main/main/*.s", "rom/SLUS_204.17.rom", 0x00400000),
    "field": ("asm/field/field/*.s", "iso/FIELD.BIN", 0x005B0000),
    "battle": ("asm/battle/battle/*.s", "iso/BATTLE.BIN", 0x00680000),
    "viewer": ("asm/viewer/viewer/*.s", "iso/VIEWER.BIN", 0x00700000),
}

COMPILER = "tools/compilers/ee-gcc2.9-991111/bin/ee-gcc"
CFLAGS = ["-O2", "-G0", "-Iinclude", "-Isrc"]
FUNC_RE = re.compile(r"^glabel (\w+)\n(.*?)^endlabel \1\n", re.M | re.S)
INSN_RE = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/", re.M)

HEADER = '#include "common.h"\n#include "m2c_macros.h"\n\n'


def find_functions(asm_glob):
    funcs = []
    for path in sorted(glob.glob(asm_glob)):
        text = open(path).read()
        for m in FUNC_RE.finditer(text):
            name, body = m.group(1), m.group(2)
            addrs = INSN_RE.findall(body)
            if not addrs:
                continue
            funcs.append({
                "name": name,
                "addr": int(addrs[0], 16),
                "insns": len(addrs),
                "asm": m.group(0),
            })
    return funcs


DEFINITION_RE = re.compile(r"^[A-Za-z_].*?\b(\w+)\(.*\)\s*\{\s*$", re.M)


def already_decompiled(segment):
    """Names of the functions src/<segment>/ defines. Only definitions count:
    a file declaring a prototype for a callee doesn't make the callee done."""
    done = set()
    for path in glob.glob(f"src/{segment}/*.c"):
        for name in DEFINITION_RE.findall(open(path).read()):
            if re.fullmatch(r"func_[0-9A-F]{8}", name) or name in SYMBOL_ADDRS:
                done.add(name)
    return done


def run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def text_words_and_relocs(obj, func):
    """Bytes of `func` in obj's .text, plus {word index: reloc type}."""
    nm = run(["mips-linux-gnu-nm", "-S", obj]).stdout
    off = size = None
    for line in nm.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[3] == func:
            off, size = int(parts[0], 16), int(parts[1], 16)
    if off is None:
        return None, None
    text = subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", obj, "/dev/stdout"],
                          capture_output=True).stdout
    words = list(struct.unpack_from("<%dI" % (size // 4), text, off))
    relocs = {}  # word index -> (reloc type, symbol name)
    rel = run(["mips-linux-gnu-readelf", "-rW", obj]).stdout
    section = None
    for line in rel.splitlines():
        if line.startswith("Relocation section"):
            section = line.split("'")[1]
            continue
        parts = line.split()
        if section == ".rel.text" and len(parts) >= 3 and re.fullmatch(r"[0-9a-f]{8}", parts[0]):
            r_off = int(parts[0], 16)
            if off <= r_off < off + size:
                relocs[(r_off - off) // 4] = (parts[2], parts[4] if len(parts) > 4 else "")
    return words, relocs


SYM_ADDR_RE = re.compile(r"^(?:func|D|jtbl)_([0-9A-F]{8})$")


def load_symbol_addrs():
    """Real names -> addresses from config/symbol_addrs*.txt, plus the file
    named by $CHECK_EXTRA_SYMS (same format), for trying out new names."""
    addrs = {}
    paths = glob.glob("config/symbol_addrs*.txt")
    if os.environ.get("CHECK_EXTRA_SYMS"):
        paths.append(os.environ["CHECK_EXTRA_SYMS"])
    for path in paths:
        for m in re.finditer(r"^(\w+)\s*=\s*0x([0-9A-Fa-f]+);", open(path).read(), re.M):
            addrs[m.group(1)] = int(m.group(2), 16)
    return addrs


SYMBOL_ADDRS = load_symbol_addrs()


def symbol_addr(name):
    m = SYM_ADDR_RE.match(name)
    if m:
        return int(m.group(1), 16)
    return SYMBOL_ADDRS.get(name)


def sext16(x):
    x &= 0xFFFF
    return x - 0x10000 if x & 0x8000 else x


def relocated_word(words, relocs, i):
    """The value word i will have once linked, or None if it can't be resolved.
    MIPS REL relocations keep their addend in the instruction; a HI16's full
    addend also needs the low half from its paired LO16."""
    rtype, sym = relocs[i]
    addr = symbol_addr(sym)
    if addr is None:
        return None
    word = words[i]
    if rtype == "R_MIPS_26":
        return (word & 0xFC000000) | (((addr + ((word & 0x03FFFFFF) << 2)) >> 2) & 0x03FFFFFF)
    if rtype == "R_MIPS_LO16":
        return (word & 0xFFFF0000) | ((addr + sext16(word)) & 0xFFFF)
    if rtype == "R_MIPS_HI16":
        lo = next((words[j] for j in sorted(relocs) if j > i and relocs[j] == ("R_MIPS_LO16", sym)), 0)
        full = addr + ((word & 0xFFFF) << 16) + sext16(lo)
        return (word & 0xFFFF0000) | (((full + 0x8000) >> 16) & 0xFFFF)
    return None


TABLE_RE = re.compile(r"^dlabel (jtbl_[0-9A-F]{8})\n.*?^enddlabel \1\n",
                      re.M | re.S)
_TABLES = None


def table_asm(names):
    """The .rodata asm of the given jump tables, so m2c can rebuild the switch."""
    global _TABLES
    if _TABLES is None:
        _TABLES = {}
        for path in glob.glob("asm/*/data/*/*.rodata.s"):
            for m in TABLE_RE.finditer(open(path).read()):
                _TABLES[m.group(1)] = m.group(0)
    return "".join(_TABLES[n] for n in sorted(names) if n in _TABLES)


def process(job):
    func, workdir, rom_path, base, context = job
    name = func["name"]
    stem = os.path.join(workdir, f"{func['addr']:08X}")
    with open(stem + ".s", "w") as f:
        f.write('.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n\n')
        f.write(func["asm"])
    inputs = [stem + ".s"]
    tables = set(re.findall(r"%lo\((jtbl_[0-9A-F]{8})\)", func["asm"]))
    if tables:
        with open(stem + ".rodata.s", "w") as f:
            f.write('.include "macro.inc"\n.section .rodata\n\n' + table_asm(tables))
        inputs.append(stem + ".rodata.s")

    m2c = run(["m2c", "-t", "mipsee-gcc-c", "--valid-syntax", "--globals", "used",
               "--context", context, *inputs])
    if m2c.returncode != 0 or not m2c.stdout.strip() or "Decompilation failure" in m2c.stdout:
        return {"name": name, "status": "m2c-failed", "detail": (m2c.stdout + m2c.stderr).strip()[-300:]}
    c_src = HEADER + m2c.stdout
    with open(stem + ".c", "w") as f:
        f.write(c_src)
    if "M2C_ERROR" in c_src:
        return {"name": name, "status": "m2c-error"}

    cc = run([COMPILER, *CFLAGS, "-S", "-o", "-", stem + ".c"])
    if cc.returncode != 0:
        return {"name": name, "status": "compile-error", "detail": cc.stderr.strip()[-300:]}
    if tables:
        # Jump tables need the full checker (it also compares the tables)
        chk = run(["python3", "tools/check_func.py", stem + ".c"])
        first = chk.stdout.splitlines()[0] if chk.stdout else chk.stderr.strip()[-200:]
        if chk.returncode == 0:
            return {"name": name, "status": "match"}
        m = re.search(r"(\d+) differing words", first)
        return {"name": name, "status": "diff" if m else "check-failed",
                "diffs": int(m.group(1)) if m else None, "detail": first}
    obj = stem + ".o"
    ok, err = compile_c(stem + ".c", obj, stem + ".compiled.s")
    if not ok:
        return {"name": name, "status": "assemble-error", "detail": err[-300:]}

    words, relocs = text_words_and_relocs(obj, name)
    if words is None:
        return {"name": name, "status": "no-symbol"}
    rom = open(rom_path, "rb").read()
    n_orig = func["insns"]
    orig = list(struct.unpack_from("<%dI" % n_orig, rom, func["addr"] - base))
    # The original's size includes alignment padding after the function, which
    # the compiler output does not; compare the real instructions and require
    # that anything left over is padding.
    if len(words) > n_orig or any(w != 0 for w in orig[len(words):]):
        return {"name": name, "status": "size-mismatch", "got": len(words), "want": n_orig}
    diffs = 0
    for i, (got, want) in enumerate(zip(words, orig)):
        if i in relocs:
            got = relocated_word(words, relocs, i)
        if got != want:
            diffs += 1
    return {"name": name, "status": "match" if diffs == 0 else "diff", "diffs": diffs}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("segment", choices=SEGMENTS)
    ap.add_argument("--max-insns", type=int, default=24)
    ap.add_argument("--min-insns", type=int, default=1)
    ap.add_argument("--only", nargs="*", help="function names to process")
    ap.add_argument("--jtbl-only", action="store_true",
                    help="only functions with jump tables (results go to results_jtbl.json)")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    args = ap.parse_args()

    asm_glob, rom_path, base = SEGMENTS[args.segment]
    workdir = f"build/m2c/{args.segment}"
    os.makedirs(workdir, exist_ok=True)

    # m2c wants its type context already preprocessed
    context = os.path.join(workdir, "context.c")
    with open(context, "w") as fp:
        fp.write(run([COMPILER, "-E", "-P", "-Iinclude", "include/common.h"]).stdout)
        # Every signature the project already knows, so drafts call functions
        # with the right argument counts and return types (tools/make_m2c_context.py)
        known = run(["python3", "tools/make_m2c_context.py"])
        if known.returncode == 0 and os.path.exists("build/m2c/known_functions.h"):
            fp.write(open("build/m2c/known_functions.h").read())

    done = already_decompiled(args.segment)
    funcs = []
    for f in find_functions(asm_glob):
        if args.only:
            if f["name"] in args.only:
                funcs.append(f)
            continue
        if f["name"] in done or not args.min_insns <= f["insns"] <= args.max_insns:
            continue
        # Jump-table functions are drafted separately (--jtbl-only)
        if ("jtbl_" in f["asm"]) != args.jtbl_only:
            continue
        # ee-gcc always aligns functions to 8 (.p2align 3), so a function at a
        # 4-aligned address was not built by it this way (likely SDK asm).
        if f["addr"] % 8:
            continue
        funcs.append(f)

    with Pool(args.j) as pool:
        results = pool.map(process, [(f, workdir, rom_path, base, context) for f in funcs])
    for f, r in zip(funcs, results):
        r["addr"] = f"{f['addr']:08X}"
        r["insns"] = f["insns"]
    with open(os.path.join(workdir, "results_jtbl.json" if args.jtbl_only else "results.json"), "w") as fp:
        json.dump(results, fp, indent=1)

    counts = {}
    for r in results:
        counts[r["status"]] = counts.get(r["status"], 0) + 1
    print(f"{len(results)} functions:", ", ".join(f"{k} {v}" for k, v in sorted(counts.items())))


if __name__ == "__main__":
    main()
