"""Shared code for the data-only C files (plan step 6b, docs/shifting.md):
the asm data pieces of a binary as splat wrote them (labels, directives,
symbolic words), the symbol addresses, and the markers a data C file uses.

Used by tools/gen_data_c.py (draft a C file from asm), tools/check_data.py
(check one C file against the original) and tools/update_segments.py (place
the files).

A data C file lives in src/<binary>/data/<ADDR>.c and claims its range of
the original .data and/or .rodata with markers near its top:

    /* data: 0x0049A868-0x0049A938 */
    /* rodata: 0x006E0198-0x006E0320 */

(start inclusive, end exclusive; ADDR is the first range's start without
the leading 00). Everything it defines is a global named after the symbol
files or D_<address>, in address order.
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from binaries import BINARIES  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

DATA_RE = re.compile(r"/\*\s*data:\s*0x([0-9A-Fa-f]+)\s*-\s*0x([0-9A-Fa-f]+)\s*\*/")
RODATA_RE = re.compile(r"/\*\s*rodata:\s*0x([0-9A-Fa-f]+)\s*-\s*0x([0-9A-Fa-f]+)\s*\*/")

ITEM_RE = re.compile(r"/\*\s+([0-9A-F]+)\s+([0-9A-F]{8})(?:\s+[0-9A-F]+)?\s+\*/\s+"
                     r"\.(word|short|byte|asciz|float|double)\s+(.*?)\s*$")
LABEL_RE = re.compile(r"^\s*(dlabel|jlabel|glabel|alabel|ehlabel)\s+(\S+)")
SYMFILE_RE = re.compile(r"^(\w+)\s*=\s*0x([0-9A-Fa-f]+);(.*)$", re.M)
SIZES = {"word": 4, "short": 2, "byte": 1, "float": 4, "double": 8}
PIECE_RE = re.compile(r"^\s*- \[0x([0-9A-Fa-f]+),\s*([.\w]+),\s*([\w/]+)\]")
END_RE = re.compile(r"^\s*- \[0x([0-9A-Fa-f]+)\]")


def natural_align(addr, cap=16):
    """The alignment the Makefile gives a piece starting at addr (capped at 16)."""
    for a in (16, 8, 4, 2):
        if a <= cap and addr % a == 0:
            return a
    return 1


def markers(text):
    """{'.data': (start, end), '.rodata': (start, end)} claimed by a data C file."""
    out = {}
    for sec, rx in ((".data", DATA_RE), (".rodata", RODATA_RE)):
        m = rx.search(text[:3000])
        if m:
            start, end = int(m.group(1), 16), int(m.group(2), 16)
            if end <= start:
                raise ValueError(f"{sec[1:]} marker: end 0x{end:X} is not after start 0x{start:X}")
            out[sec] = (start, end)
    return out


def section_ranges(b):
    """{'.data': (lo, hi), '.rodata': (lo, hi)} (vram) of a binary's data that
    data C files may claim."""
    cfg = BINARIES[b]
    out = {}
    if cfg.get("data"):
        out[".data"] = tuple(cfg["vram"] + x for x in cfg["data"])
    out[".rodata"] = tuple(cfg["vram"] + x for x in cfg["rodata"])
    return out


def binary_for_data(addr):
    """The binary whose .data or .rodata holds addr."""
    for name in BINARIES:
        for sec, (lo, hi) in section_ranges(name).items():
            if lo <= addr < hi:
                return name, sec
    raise KeyError(f"0x{addr:08X} is not in any binary's .data or .rodata")


def rom(b):
    cfg = BINARIES[b]
    return open(os.path.join(ROOT, cfg["rom"]), "rb").read()


def rom_bytes(b, start, end, image=None):
    cfg = BINARIES[b]
    image = image if image is not None else rom(b)
    return image[start - cfg["vram"]:end - cfg["vram"]]


def config_pieces(b):
    """Subsegments of the binary's config in order: (start vram, end vram, type, name).
    The end is the next subsegment's start (or the end marker / section end)."""
    cfg = BINARIES[b]
    vram = cfg["vram"]
    rows = []
    stop = None
    for line in open(os.path.join(ROOT, cfg["config"])):
        m = PIECE_RE.match(line)
        if m:
            rows.append([vram + int(m.group(1), 16), None, m.group(2), m.group(3)])
            continue
        m = re.match(r"\s*- \{\s*type:\s*(\.?bss),\s*vram:\s*0x([0-9A-Fa-f]+)", line)
        if m and rows and stop is None:
            rows.append([int(m.group(2), 16), None, m.group(1), None])
            continue
        m = END_RE.match(line)
        if m and stop is None:
            stop = vram + int(m.group(1), 16)
        if re.match(r"^\s*- name:", line) and rows:
            break  # main: the .lay_subprog1 segment follows
    ends = section_ranges(b)
    sec_end = max(hi for _, hi in ends.values())
    for i, r in enumerate(rows):
        r[1] = rows[i + 1][0] if i + 1 < len(rows) else (stop or sec_end)
    return [tuple(r) for r in rows]


def asm_path(b, typ, name):
    return os.path.join(ROOT, f"asm/{b}/data/{name}.{typ}.s")


class AsmPiece:
    """One asm data piece: labels {vram: [names]}, items [(vram, kind, operand)]."""

    def __init__(self, b, start, end, typ, name):
        self.b, self.start, self.end, self.typ, self.name = b, start, end, typ, name
        self.section = "." + typ
        self.path = asm_path(b, typ, name)
        self.labels = {}
        self.items = []
        pending = []
        for line in open(self.path):
            m = LABEL_RE.match(line)
            if m:
                pending.append(m.group(2))
                continue
            m = ITEM_RE.search(line)
            if m:
                v = int(m.group(2), 16)
                for n in pending:
                    self.labels.setdefault(v, []).append(n)
                pending = []
                self.items.append((v, m.group(3), m.group(4)))
        for n in pending:  # a label at the very end
            self.labels.setdefault(self.end, []).append(n)

    def symbolic_words(self):
        """{vram: operand} for every .word that splat wrote as a symbol expression."""
        out = {}
        for v, kind, op in self.items:
            if kind == "word" and not re.fullmatch(r"-?(0x[0-9A-Fa-f]+|\d+)", op):
                out[v] = op
        return out


def asm_pieces(b, start=None, end=None):
    """The asm data/rodata pieces of a binary (overlapping [start, end) if given)."""
    out = []
    for s, e, typ, name in config_pieces(b):
        if typ not in ("data", "rodata") or name is None:
            continue
        if start is not None and (e <= start or s >= end):
            continue
        if not os.path.exists(asm_path(b, typ, name)):
            continue
        out.append(AsmPiece(b, s, e, typ, name))
    return out


def parse_expr(op):
    """'D_006E0200 + 0xD' -> ('D_006E0200', 0xD)."""
    m = re.fullmatch(r"([\w.$]+)\s*(?:([+-])\s*(0x[0-9A-Fa-f]+|\d+))?", op.strip())
    if not m:
        raise ValueError(f"can't parse symbol expression {op!r}")
    off = 0
    if m.group(2):
        off = int(m.group(3), 0) * (1 if m.group(2) == "+" else -1)
    return m.group(1), off


class Symbols:
    """Addresses of the names a binary's data can refer to: the symbol files
    (its own and the main executable's), config/memory_map.ld, and
    D_/func_/.L names (address in the name)."""

    def __init__(self, b):
        self.b = b
        self.addr = {}
        self.funcs = set()
        files = ["config/symbol_addrs.txt"]
        if b != "main":
            files.insert(0, f"config/symbol_addrs.{b}.txt")
        for f in files:
            for name, a, rest in SYMFILE_RE.findall(open(os.path.join(ROOT, f)).read()):
                self.addr.setdefault(name, int(a, 16))
                if "type:func" in rest:
                    self.funcs.add(name)
        # Fixed addresses (load addresses, GBX slots, ...): data, not code
        self.memory_map = set()
        text = re.sub(r"/\*.*?\*/", "", open(os.path.join(ROOT, "config/memory_map.ld")).read(), flags=re.S)
        for name, a in re.findall(r"^\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", text, re.M):
            self.addr.setdefault(name, int(a, 16))
            self.memory_map.add(name)

    def lookup(self, name):
        m = re.fullmatch(r"(?:D_|func_|\.L|L_)([0-9A-F]{6,8})", name)
        if name in self.addr:
            return self.addr[name]
        if m:
            return int(m.group(1), 16)
        return None

    def is_code(self, name):
        if name in self.memory_map:
            return False
        if name in self.funcs or name.startswith(("func_", ".L")):
            return True
        a = self.lookup(name)
        for cfg in BINARIES.values():
            if a is not None and cfg["vram"] <= a < cfg["vram"] + cfg["text_end"]:
                return True
        return False


def data_c_files(b):
    """src/<binary>/data/*.c"""
    return sorted(glob.glob(os.path.join(ROOT, f"src/{b}/data/*.c")))


def defined_objects(text):
    """Names of the global objects a data C file defines (not statics), in
    order: every top-level `<type> <declarator> = ...;`."""
    # strings, character constants and comments in one pass (each may hold the others)
    text = re.sub(r'"(?:[^"\\\n]|\\.)*"|\'(?:[^\'\\\n]|\\.)*\'|/\*.*?\*/|//[^\n]*',
                  lambda m: '""' if m.group(0)[0] in "\"'" else " ", text, flags=re.S)
    text = re.sub(r"^\s*#[^\n]*", "", text, flags=re.M)
    prev = None
    while prev != text:  # flatten braces (struct bodies, initializers)
        prev = text
        text = re.sub(r"\{[^{}]*\}", "{}", text)
    text = re.sub(r"__attribute__\s*\(\((?:[^()]|\([^()]*\))*\)\)", "", text)
    names = []
    for stmt in text.split(";"):
        stmt = stmt.strip()
        if "=" not in stmt or re.match(r"(static|extern|typedef)\b", stmt):
            continue
        decl = stmt.split("=", 1)[0]
        m = re.search(r"\(\s*\*\s*(?:const\s+)?(\w+)", decl)  # (*name[...])(...)
        if not m:
            m = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*$", decl.strip())
        if m:
            names.append(m.group(1))
    return names


def set_alignment(c_file, obj):
    """Give a data C file's .data/.rodata the natural alignment of its range's
    start (capped at 16), like every other piece (see the Makefile), so the
    padding before it is recreated. tools/check_data.py checks that gcc's own
    alignment of the objects divides the start. A code file that claims a
    .rodata range (`/* rodata: */`, tools/compile_c.py) gets the same for its
    .rodata: the zeros a data file leaves before it are recreated."""
    import subprocess
    text = open(c_file).read()
    if "/data/" in c_file.replace(os.sep, "/"):
        claims = markers(text)
    else:
        from compile_c import owned_rodata
        owned = owned_rodata(text)
        claims = {".rodata": owned} if owned else {}
    args = []
    for sec, (start, _) in claims.items():
        args += ["--set-section-alignment", f"{sec}={natural_align(start)}"]
    if args:
        subprocess.run(["mips-linux-gnu-objcopy", *args, obj], check=True)


def main():
    if len(sys.argv) == 4 and sys.argv[1] == "align":
        set_alignment(sys.argv[2], sys.argv[3])
    else:
        raise SystemExit("usage: data_asm.py align <file.c> <file.o>   (used by the Makefile, for data files\n"
                         "       and code files with a /* rodata: */ marker)")


if __name__ == "__main__":
    main()
