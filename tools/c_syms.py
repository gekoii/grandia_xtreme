#!/usr/bin/env python3
"""Write a linker script defining every name the code uses that no object
defines, so that each follows the layout (see docs/shifting.md).

C files and asm pieces refer to globals and functions by address-derived
names (D_004A948C, func_00412910) or by names from the symbol files. Some of
those have no label of their own: a field inside another object (a byte of
a word-typed table), a string inside another C file's .rodata, an alternate
name for a label, an address-taken code label in another asm piece. For each
one this emits a PROVIDE (used only when nothing else defines the name):

  own binary   relative to the label or object that contains the address:
               PROVIDE(menuTextAddG = menuTextAddR + 0x1);
               PROVIDE(D_004F0D00 = sysState);
               PROVIDE(ptclDataFileName = __rodata_main_4019F8 + 0x0);
               (__rodata_<file> marks where a C file's .rodata starts, see
               tools/ld_script.py), so it moves when the layout does.
  main, from an overlay   the value the built main executable has for it
               (--main-elf): an overlay is a separate file holding absolute
               main addresses, so it is relinked against main, not shifted
               with it. Names main doesn't define are taken relative to the
               main label containing them, as above.
  elsewhere    the fixed address: hardware registers, scratchpad, constants
               splat named (D_10000), other overlays. Fixed areas of the
               memory map are defined in config/memory_map.ld instead, and
               computed names in config/symbol_exprs.ld; this script leaves
               the names those define alone.

Usage: c_syms.py <out.ld> <dir>... [--names <symbol file>...]
                 [--binary <name>] [--main-elf <elf>]
  Scans *.c and *.s under each dir for names; --names also provides every
  name in those symbol files. --binary defaults to the binary whose src/ or
  asm/ folder is given.
"""
import bisect
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from binaries import BINARIES  # noqa: E402

NAME_RE = re.compile(r"\b((?:D|func)_([0-9A-F]{4,8}))\b")
# Code labels whose address is taken (%hi/%lo). splat writes them as local
# .L labels, which can't be resolved once their function and the user end
# up in different asm pieces.
LABEL_RE = re.compile(r"%(?:hi|lo)\((\.L([0-9A-F]{8}))\)")
SYMFILE_RE = re.compile(r"^(\w+)\s*=\s*0x([0-9A-Fa-f]+);", re.M)
LABEL_DEF_RE = re.compile(r"^\s*(?:glabel|alabel|dlabel)\s+(\w+)")
VRAM_COMMENT_RE = re.compile(r"/\*\s+(?:[0-9A-F]+\s+)?([0-9A-F]{8})\b")
C_DEF_RE = re.compile(r"^(?!static\b)[A-Za-z_].*?\b(\w+)\(.*\)\s*\{\s*$", re.M)
# An object definition in a src/<binary>/bss_*.c file (tools/gen_bss.py):
# `<type> <name>[<size>] BSS ...;`
C_BSS_RE = re.compile(r"^(?!static\b|extern\b|typedef\b)[A-Za-z_][\w\s*]*?\b(\w+)\s*(?:\[[^\]]*\]\s*)*BSS\b[^;]*;", re.M)
FIXED_FILES = ["config/memory_map.ld", "config/symbol_exprs.ld"]

# Each binary's own memory: code and data, up to the end of its .bss.
MEMORY = {
    "main": [(0x00400000, 0x005A6B20), (0x00700000, 0x00700080)],  # + the .lay_subprog1 stub
    "field": [(0x005B0000, 0x0067F646)],
    "battle": [(0x00680000, 0x006F9350)],
    "viewer": [(0x00700000, 0x0070DF44)],
}


def in_memory(b, addr):
    return any(lo <= addr < hi for lo, hi in MEMORY[b])


def symbol_file_names(paths):
    names = {}
    for path in paths:
        for name, addr in SYMFILE_RE.findall(open(path).read()):
            names.setdefault(name, int(addr, 16))
    return names


def fixed_names():
    """Names config/memory_map.ld and config/symbol_exprs.ld define."""
    out = set()
    for path in FIXED_FILES:
        if os.path.exists(path):
            text = re.sub(r"/\*.*?\*/", "", open(path).read(), flags=re.S)
            out.update(re.findall(r"^\s*(?:PROVIDE\s*\(\s*)?([\w.]+)\s*=", text, re.M))
    return out


class LabelIndex:
    """Where each defined label of one binary is, from its asm, its C
    function definitions and its C files' .rodata pieces."""

    def __init__(self, b):
        self.b = b
        self.labels = []  # (addr, name, piece) - piece = (start, end) of the object
        cfg = BINARIES[b]
        vram = cfg["vram"]
        # Pieces (objects) from the splat config, in order: (vram, type, name)
        pieces = []
        for line in open(cfg["config"]):
            m = re.match(r"\s*- \[0x([0-9A-Fa-f]+),\s*([.\w]+),\s*([\w/]+)\]", line)
            if m:
                pieces.append((int(m.group(1), 16), m.group(2), m.group(3)))
            elif re.match(r"\s*- \{\s*type:\s*\.?bss,\s*vram:\s*0x([0-9A-Fa-f]+)", line):
                v = int(re.search(r"vram:\s*0x([0-9A-Fa-f]+)", line).group(1), 16)
                pieces.append((v - vram, "bss", None))
        bounds = sorted({off for off, _, _ in pieces})
        self.piece_starts = [vram + off for off in bounds]

        # asm labels
        for path in glob.glob(f"asm/{b}/**/*.s", recursive=True):
            lines = open(path).read().splitlines()
            start = self._file_start(path)
            for i, line in enumerate(lines):
                m = LABEL_DEF_RE.match(line)
                if not m:
                    continue
                addr = None
                for nxt in lines[i + 1:i + 6]:
                    c = VRAM_COMMENT_RE.search(nxt)
                    if c:
                        addr = int(c.group(1), 16)
                        break
                    if LABEL_DEF_RE.match(nxt):
                        break
                if addr is None:
                    addr = start  # e.g. a textbin: label at the start of the piece
                if addr is not None:
                    self.labels.append((addr, m.group(1), self.piece_of(addr)))
        # C functions, by the names the C files define them under
        named = symbol_file_names(sorted(glob.glob("config/symbol_addrs*.txt")))
        for path in glob.glob(f"src/{b}/*.c"):
            for name in C_DEF_RE.findall(open(path).read()):
                if re.fullmatch(r"func_[0-9A-F]{8}", name):
                    addr = int(name[5:], 16)
                elif name in named:
                    addr = named[name]
                else:
                    continue
                self.labels.append((addr, name, self.piece_of(addr)))
        # C .bss objects (src/<binary>/bss_*.c), by their symbol-file or D_ names
        own_names = symbol_file_names([f for f in (f"config/symbol_addrs.{b}.txt", "config/symbol_addrs.txt")
                                       if os.path.exists(f)])
        for path in glob.glob(f"src/{b}/bss_*.c"):
            for name in C_BSS_RE.findall(open(path).read()):
                if re.fullmatch(r"D_[0-9A-F]{8}", name):
                    addr = int(name[2:], 16)
                elif name in own_names:
                    addr = own_names[name]
                else:
                    addr = None
                if addr is None or not in_memory(b, addr):
                    raise SystemExit(f"{path}: {name} has no address in this binary in the symbol files")
                self.labels.append((addr, name, self.piece_of(addr)))
        # Objects defined by data-only C files (src/<binary>/data/*.c, plan
        # step 6b), by their symbol-file or D_ names
        from data_asm import defined_objects
        for path in sorted(glob.glob(f"src/{b}/data/*.c")):
            for name in defined_objects(open(path).read()):
                if re.fullmatch(r"D_[0-9A-F]{8}", name):
                    addr = int(name[2:], 16)
                elif name in own_names:
                    addr = own_names[name]
                else:
                    addr = None
                if addr is None or not in_memory(b, addr):
                    raise SystemExit(f"{path}: {name} has no address in this binary in the symbol files")
                self.labels.append((addr, name, self.piece_of(addr)))
        # C files' .rodata pieces start at their marker symbol
        # (__rodata_<binary>_<file>, or __rodata_<binary>_data_<file> for a data file)
        for (off, typ, name) in pieces:
            if typ == ".rodata":
                self.labels.append((vram + off, "__rodata_" + name.replace("/", "_"), self.piece_of(vram + off)))
        self.labels.sort()
        self.addrs = [a for a, _, _ in self.labels]
        self.names = {n for _, n, _ in self.labels}
        self.by_name = {n: a for a, n, _ in self.labels}

    def _file_start(self, path):
        m = re.search(r"/([0-9A-F]{6,8})(?:\.\w+)?\.s$", path)
        if not m:
            return None
        v = int(m.group(1), 16)
        return v if len(m.group(1)) == 8 else v + BINARIES[self.b]["vram"]

    def piece_of(self, addr):
        i = bisect.bisect_right(self.piece_starts, addr) - 1
        lo = self.piece_starts[i] if i >= 0 else None
        hi = self.piece_starts[i + 1] if i + 1 < len(self.piece_starts) else None
        return (lo, hi)

    def resolve(self, name, addr):
        """(label, offset) of the label that contains addr in the same object,
        or None. An exact label of another name gives offset 0."""
        piece = self.piece_of(addr)
        i = bisect.bisect_right(self.addrs, addr) - 1
        best = None
        while i >= 0:
            a, n, p = self.labels[i]
            if p != piece:
                break
            if n != name and not n.startswith(".L"):
                best = (n, addr - a)
                # prefer a label exactly at addr, then the nearest below it
                break
            i -= 1
        return best


def main():
    args = sys.argv[2:]
    opts = {"--names": [], "--binary": [], "--main-elf": []}
    dirs = []
    key = None
    for a in args:
        if a in opts:
            key = a
        elif key:
            opts[key].append(a)
        else:
            dirs.append(a)
    out = sys.argv[1]
    b = opts["--binary"][0] if opts["--binary"] else None
    if b is None:
        for d in dirs:
            parts = d.rstrip("/").split("/")
            if len(parts) >= 2 and parts[-2] in ("src", "asm") and parts[-1] in BINARIES:
                b = parts[-1]
    if b is None:
        raise SystemExit("c_syms.py: can't tell which binary this is; pass --binary")

    names = symbol_file_names(opts["--names"])
    paths = []
    for d in dirs:
        paths += glob.glob(os.path.join(d, "**", "*.c"), recursive=True)
        paths += glob.glob(os.path.join(d, "**", "*.s"), recursive=True)
    for path in paths:
        text = open(path).read()
        for name, addr in NAME_RE.findall(text):
            names.setdefault(name, int(addr, 16))
        for name, addr in LABEL_RE.findall(text):
            names.setdefault(name, int(addr, 16))

    own = LabelIndex(b)
    fixed = fixed_names()
    main_syms = main_index = None
    if opts["--main-elf"]:
        import elf32
        main_syms = {}
        for s in elf32.Elf(opts["--main-elf"][0]).symbols():
            if s["name"] and s["shndx"] != elf32.SHN_UNDEF and re.fullmatch(r"[\w.]+", s["name"]):
                main_syms.setdefault(s["name"], s["value"])
        main_index = LabelIndex("main")

    lines = []
    counts = {"relative": 0, "main": 0, "fixed": 0}
    for name in sorted(names):
        addr = names[name]
        if name in fixed or name in own.names:
            continue
        expr = None
        if in_memory(b, addr):
            r = own.resolve(name, addr)
            if r:
                expr = r[0] if r[1] == 0 else f"{r[0]} + 0x{r[1]:X}"
                counts["relative"] += 1
        elif main_syms is not None and in_memory("main", addr):
            if name in main_syms:
                expr = f"0x{main_syms[name]:08X}"
            else:
                r = main_index.resolve(name, addr)
                if r and r[0] in main_syms:
                    expr = f"0x{main_syms[r[0]] + r[1]:08X}"
            if expr:
                counts["main"] += 1
        if expr is None:
            expr = f"0x{addr:08X}"
            counts["fixed"] += 1
        lines.append(f"PROVIDE({name} = {expr});")

    os.makedirs(os.path.dirname(out) or ".", exist_ok=True)
    with open(out, "w") as f:
        f.write(f"/* Generated by tools/c_syms.py for {b}: names no object defines "
                f"({counts['relative']} relative to a label, {counts['main']} from the main executable, "
                f"{counts['fixed']} fixed) */\n")
        f.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
