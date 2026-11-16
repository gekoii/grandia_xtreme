#!/usr/bin/env python3
"""The linker scripts the build links with, and the link inputs per binary.

Usage: ld_script.py <binary> <out.ld>

splat writes linker_scripts/<binary>.ld. The build links with a copy in
build/ld/ that also asserts that the binary's load address is the one in
config/memory_map.ld.

tools/shift_test.py uses the same copy with padding inserted (write_script).
link_scripts() and c_syms_args() list what the Makefile links each binary
with; shift_test.py relinks with exactly those, and checks that its
baseline link equals the normal build.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from binaries import BINARIES  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))


def elf_name(b):
    return "SLUS_204.17" if b == "main" else b


def source_script(b):
    return f"linker_scripts/{elf_name(b)}.ld"


# ------------------------------------------------------------------ link inputs
# Keep these in step with the Makefile's link rules.
def c_syms_path(b):
    return "build/c_syms.ld" if b == "main" else f"build/c_syms.{b}.ld"


def c_syms_args(b, out, main_elf=None):
    """Command line (after `python3`) that writes binary b's c_syms script;
    an overlay's takes the main executable's addresses from main_elf."""
    args = ["tools/c_syms.py", out, f"src/{b}", f"asm/{b}", "--names", "config/symbol_addrs.txt"]
    if b != "main":
        args += [f"config/symbol_addrs.{b}.txt", "--main-elf", main_elf or "build/SLUS_204.17.elf"]
    return args


def link_scripts(b, script, csyms):
    """The -T arguments for linking binary b with `script` as its main script."""
    out = ["-T", script, "-T", "config/memory_map.ld", "-T", "config/symbol_exprs.ld"]
    if b == "main":
        out += ["-T", "config/linker_script_extra.ld"]
    return out + ["-T", csyms]


# Each binary's load address must be the one in config/memory_map.ld
VRAM_ASSERTS = {
    "main": [("main_VRAM", "MAIN_VRAM"), ("lay_subprog1_VRAM", "SUBPROG_VRAM")],
    "field": [("field_VRAM", "FIELD_VRAM")],
    "battle": [("battle_VRAM", "BATTLE_VRAM")],
    "viewer": [("viewer_VRAM", "SUBPROG_VRAM")],
}


# ------------------------------------------------------------------ padding
INPUT_RE = re.compile(r"^(\s*)(build/(\S+)\.o)\((\.\w+)[^)]*\);\s*$")


def point_symbol(p):
    return "__shift_" + re.sub(r"\W", "_", p)


def map_sections(path):
    """Input sections from a linker map: (addr, size, section, object), sorted."""
    out = []
    pending = None
    line_re = re.compile(r"^ (\.[\w.$]+)?\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+\.o)\s*$")
    for line in open(os.path.join(ROOT, path)):
        m = re.match(r"^ (\.[\w.$]+)\s*$", line)
        if m:
            pending = m.group(1)
            continue
        m = line_re.match(line)
        if m:
            sec = m.group(1) or pending
            pending = None
            addr, size = int(m.group(2), 16), int(m.group(3), 16)
            if size and addr and sec:
                out.append((addr, size, sec, m.group(4)))
            continue
        pending = None
    out.sort()
    return out


def find_point(b, lines, p):
    """Index of the script line before which point p's marker (and padding) goes."""
    seg = b
    if p == "text":
        start = lines.index(f"        {seg}_TEXT_START = .;")
        for i in range(start + 1, len(lines)):
            if INPUT_RE.match(lines[i]):
                return i + 1
        raise SystemExit(f"{b}: no .text input after {seg}_TEXT_START")
    # The start of a section: splat's marker before its first input (every
    # binary's data, rodata and bss are separate subsegments, see
    # docs/shifting.md; for the main executable and FIELD/BATTLE, bss starts
    # with .scommon).
    marker = {"data": "DATA_START", "rodata": "RODATA_START", "bss": "BSS_START"}.get(p)
    if marker:
        return lines.index(f"        {seg}_{marker} = .;") + 1
    addr = int(p, 16)
    # The first input section at or after addr, from the normal build's map.
    for a, size, sec, obj in map_sections(f"build/{elf_name(b)}.map"):
        if a >= addr:
            for i, line in enumerate(lines):
                m = INPUT_RE.match(line)
                if m and m.group(2) == obj and sec.startswith(m.group(4)):
                    return i
            raise SystemExit(f"{b}: no script line for {obj}({sec})")
    return None  # no input section starts there


def script_lines(b):
    """splat's linker script for b with the load-address checks added."""
    out = open(os.path.join(ROOT, source_script(b))).read().splitlines()
    if b == "main":
        # .scommon and .bss share splat's NOLOAD section, but only .bss is
        # 128-aligned (its first object, src/main/bss_4A9680.c): start the
        # section where .rodata ends, not at the inputs' largest alignment
        i = out.index("    .main_bss (NOLOAD) :")
        out[i] = "    .main_bss . (NOLOAD) :"
    for sym, const in VRAM_ASSERTS[b]:
        out.append(f'ASSERT({sym} == {const}, "{sym} differs from {const} in config/memory_map.ld");')
    return out


def write_script(b, out, points=(), pads=None):
    """Write b's linker script to out, with a __shift_<point> symbol at each of
    `points` and `. += size;` after the ones in `pads` (point -> size)."""
    pads = pads or {}
    lines = script_lines(b)
    inserts = []
    for p in points:
        i = find_point(b, lines, p)
        if i is not None:
            inserts.append((i, p))
    for i, p in sorted(inserts, reverse=True):
        new = [f"        {point_symbol(p)} = .;"]
        if p in pads:
            new.append(f"        . += {pads[p]:#x};")
        lines[i:i] = new
    os.makedirs(os.path.dirname(os.path.join(ROOT, out)) or ".", exist_ok=True)
    with open(os.path.join(ROOT, out), "w") as f:
        f.write("\n".join(lines) + "\n")


def main():
    if len(sys.argv) != 3 or sys.argv[1] not in BINARIES:
        raise SystemExit(__doc__)
    write_script(sys.argv[1], sys.argv[2])


if __name__ == "__main__":
    main()
