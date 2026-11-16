#!/usr/bin/env python3
"""Compile one C file to an object the way the game's build did.

The game was not built with a single toolchain. Most of its C matches
ee-gcc 2.9-ee-991111 assembled with the ee-gcc 2.96 toolchain's binutils
(the default here), but some files only match with another compiler build
or assembler. A file picks those with marker comments near its top:

    /* cc: ee-gcc2.9-991111-01 */   compiler (a tools/compilers/ directory)
    /* as: ee-gcc2.9-991111 */      assembler (a tools/compilers/ directory)
    /* as: ee-gcc2.96-hazard */     the 2.96 assembler plus the hazard nop the
                                    game's assembler added at `.set noreorder`
                                    (tools/as_hazard.py)
    /* cflags: -fno-gcse */          extra -f options (some files were built
                                     with an optimization switched off)

A file whose functions keep constants or strings in .rodata (double literals,
static const tables) claims that range of the original .rodata with

    /* rodata: 0x004A7910-0x004A7920 */   (start inclusive, end exclusive)

tools/update_segments.py then places the file's .rodata there and
tools/check_func.py compares it with the original bytes. Without the marker a
file may only emit jump tables, which are placed automatically.

For example, two nops before every div.s come from the 991111-01 compiler,
and nops after mfc1/mtc1 or between c.cond.s and bc1t come from the 1999
assembler shipped with 991111.

Usage: compile_c.py <file.c> <out.o> [-S <out.s>]
Importable: compile_c(c_path, obj, asm_out=None) -> (ok, error text).
"""
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from as_hazard import add_hazard_nops  # noqa: E402

CFLAGS = ["-O2", "-G0", "-Iinclude", "-Isrc"]
DEFAULT_CC = "ee-gcc2.9-991111"
DEFAULT_AS = "ee-gcc2.96"
# Not a directory: the 2.96 assembler after tools/as_hazard.py, which adds the
# hazard nop the 1999 assembler puts at `.set noreorder` (see that file)
HAZARD_AS = "ee-gcc2.96-hazard"
ASFLAGS = ["-EL", "-G0", "-O2", "-mabi=eabi"]  # what the ee-gcc driver passes
MARKER_RE = re.compile(r"/\*\s*(cc|as):\s*([\w.-]+)\s*\*/")
CFLAGS_RE = re.compile(r"/\*\s*cflags:\s*([^*]*?)\s*\*/")
FLAG_RE = re.compile(r"-f[a-z0-9-]+$")
RODATA_RE = re.compile(r"/\*\s*rodata:\s*0x([0-9A-Fa-f]+)\s*-\s*0x([0-9A-Fa-f]+)\s*\*/")


def owned_rodata(text):
    """(start, end) vram range of .rodata a C file claims with a marker, or None."""
    m = RODATA_RE.search(text[:2000])
    if not m:
        return None
    start, end = int(m.group(1), 16), int(m.group(2), 16)
    if end <= start:
        raise ValueError(f"rodata marker: end 0x{end:X} is not after start 0x{start:X}")
    return start, end


def toolchain(c_path):
    """(compiler dir, assembler dir, extra flags) for a C file, from its marker comments."""
    picks = {}
    with open(c_path) as f:
        head = f.read(2000)
    for key, value in MARKER_RE.findall(head):
        picks.setdefault(key, value)
    flags = []
    for group in CFLAGS_RE.findall(head):
        for flag in group.split():
            if not FLAG_RE.match(flag):
                raise ValueError(f"{c_path}: only -f options are allowed in a cflags marker, not {flag!r}")
            flags.append(flag)
    return picks.get("cc", DEFAULT_CC), picks.get("as", DEFAULT_AS), flags


def run(cmd):
    return subprocess.run(cmd, capture_output=True, text=True)


def compile_c(c_path, obj, asm_out=None):
    try:
        cc, asm, extra = toolchain(c_path)
    except ValueError as e:
        return False, str(e)
    asm_path = asm_out or obj + ".s"
    r = run([f"tools/compilers/{cc}/bin/ee-gcc", *CFLAGS, *extra, "-S", "-o", asm_path, c_path])
    if r.returncode:
        return False, "compile error:\n" + r.stderr.strip()
    if asm == HAZARD_AS:
        # The game's own assembler: 2.96 plus the noreorder hazard nop
        with open(asm_path) as f:
            text = add_hazard_nops(f.read())
        with open(asm_path, "w") as f:
            f.write(text)
        asm = DEFAULT_AS
    r = run([f"tools/compilers/{asm}/ee/bin/as", *ASFLAGS, "-o", obj + ".ee", asm_path])
    if r.returncode:
        return False, "assemble error:\n" + r.stderr.strip()
    # The old assemblers' symbol tables are rejected by modern ld; modern
    # objcopy rewrites them (and renames .rdata to .rodata).
    r = run(["mips-linux-gnu-objcopy", "--rename-section", ".rdata=.rodata", obj + ".ee", obj])
    os.remove(obj + ".ee")
    if not asm_out and os.path.exists(asm_path):
        os.remove(asm_path)
    if r.returncode:
        return False, "objcopy error:\n" + r.stderr.strip()
    return True, ""


def main():
    args = sys.argv[1:]
    asm_out = None
    if "-S" in args:
        i = args.index("-S")
        asm_out = args[i + 1]
        del args[i:i + 2]
    c_path, obj = args
    ok, err = compile_c(c_path, obj, asm_out)
    if not ok:
        sys.exit(err)


if __name__ == "__main__":
    main()
