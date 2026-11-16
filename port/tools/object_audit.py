#!/usr/bin/env python3
"""Find code and data that reach from one object into its neighbours
(port/README.md, "Objects that rely on their neighbours").

    python port/tools/object_audit.py            # writes build/port/object_audit.txt
    python port/tools/object_audit.py -j 8 --map build/port_x/grandia.map

The matching build keeps every object at its original address, in the
original order, so code that runs past the end of one object into the next
works there. The port's clang and lld place objects as they like: in
practice one file's objects stay in order within one section, but nothing
promises it, and objects in different files or sections end up anywhere.

**The original layout** comes from the matching build's ELFs
(build/SLUS_204.17.elf and build/<overlay>.elf): every symbol with its
address and size; the four binaries' address ranges don't overlap. A name
the linker defines (build/c_syms*.ld, `D_x = base + off`) has its address
there too, so every access resolves to an original address and the object
it falls in.

**What the port's code does** comes from each game source's generated
copy (build.generate(), as proto_audit.py does), in three clang runs:

1. `-S -O1 -fno-auto-import`: the port's own code generation, but with
   extern data addressed directly (mingw's default loads its address from
   a .refptr slot, which hides the offsets), so every constant offset from
   a data symbol is in the assembly, `_x+6680` or `.long _x+24`, with its
   source line from `.loc`. An address that lands outside the object the
   symbol is in, in the original layout, reaches a neighbour (kinds b, d).
   One with an index register (`_x+16(,%eax,4)`) needn't (a biased base,
   `a[i - 1]`), so those are listed apart, as are offsets exactly at the
   end (an end pointer, or the first byte of the next object).
2. `-fsyntax-only -ast-dump`: every file-scope declaration of a variable
   and every block-scope `extern` one; casts of an object's address to a
   pointer to something; calls that pass an object's address with a
   constant (or sizeof) argument; clang's -Warray-bounds warnings.
3. The same with probes appended, `typedef char p[sizeof(x)]`, for the
   size of each declaration as that file sees it and of each cast's and
   sizeof's type.

Kinds in the report:

  a  a name declared, and used, with a type bigger than its definition
  b  a constant index or offset past the end of an object
  c  a type, declared for an object or cast to, that spans several
     objects of the original layout
  d  an object's address with an offset beyond its size: pointer
     arithmetic, `(&x)[1]`, a size passed along with the address, a
     pointer in a data table, a name the linker defines past its base

Each case lists the objects involved, in original order with addresses,
sizes and the port source that defines each, the access sites, whether the
port's layout (build/port/grandia.map, or --map) happens to keep the same
distances, and whether port/tools/fixups/objects.py already makes them one
block.
"""

import argparse
import bisect
import collections
import multiprocessing
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import build  # noqa: E402
import syms  # noqa: E402

ROOT = build.ROOT
REPORT = os.path.join(ROOT, "build", "port", "object_audit.txt")
ELFS = {"main": "SLUS_204.17.elf", "field": "field.elf", "battle": "battle.elf", "viewer": "viewer.elf"}

# ---------------------------------------------------------------------------
# The original layout


class Obj:
    __slots__ = ("name", "addr", "size", "binary", "local", "func", "file")

    def __init__(self, name, addr, size, binary, local, func, file):
        self.name, self.addr, self.size = name, addr, size
        self.binary, self.local, self.func, self.file = binary, local, func, file

    @property
    def end(self):
        return self.addr + self.size


def read_elf_symbols(path):
    """[(name, value, size, type, bind, shndx, file)], {shndx: (name, addr, size, flags)};
    file is the STT_FILE name a local symbol follows"""
    with open(path, "rb") as f:
        elf = f.read()
    shoff, = struct.unpack_from("<I", elf, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", elf, 0x2E)
    sec = [struct.unpack_from("<IIIIIIIIII", elf, shoff + i * shentsize) for i in range(shnum)]

    def cstr(off):
        return elf[off:elf.index(b"\0", off)].decode("latin-1")

    secs = {i: (cstr(sec[shstrndx][4] + s[0]), s[3], s[5], s[2]) for i, s in enumerate(sec)}
    out = []
    for s in sec:
        if s[1] != 2:  # SHT_SYMTAB
            continue
        strtab = sec[s[6]][4]
        cur_file = None
        for k in range(s[5] // 16):
            st_name, value, size, info, _, shndx = struct.unpack_from("<IIIBBH", elf, s[4] + k * 16)
            if not st_name:
                continue
            name = cstr(strtab + st_name)
            if info & 0xF == 4:
                cur_file = name
                continue
            out.append((name, value, size, info & 0xF, info >> 4, shndx, cur_file if info >> 4 == 0 else None))
    return out, secs


class Layout:
    """Every object of the four binaries at its original address"""

    def __init__(self, root):
        objs = []
        self.addr = {}                                   # global name -> address
        self.locals = collections.defaultdict(list)      # local name -> [Obj]
        self.ranges = []
        loaded = {}
        for binary, fn in ELFS.items():
            path = os.path.join(root, "build", fn)
            if not os.path.exists(path):
                raise SystemExit(f"{path} not found: run the matching build first")
            loaded[binary] = read_elf_symbols(path)
            for i, (n, a, sz, fl) in loaded[binary][1].items():
                if fl & 2 and sz and n != ".lay_subprog1":    # SHF_ALLOC
                    self.ranges.append((a, a + sz, binary))
        for binary, (symbols, secs) in loaded.items():
            own = {i for i, (n, a, sz, fl) in secs.items() if fl & 2 and sz and n != ".lay_subprog1"}
            for name, value, size, typ, bind, shndx, file in symbols:
                if typ == 3 or name.startswith((".L", "$")):     # sections, local labels
                    continue
                if shndx in own:
                    o = Obj(name, value, size, binary, bind == 0, typ == 2, file)
                    if size:
                        objs.append(o)
                    if bind == 0:
                        self.locals[name].append(o)
                    elif binary == self.binary_of(value):
                        self.addr.setdefault(name, value)
                elif shndx == 0xFFF1 and bind != 0 and self.binary_of(value):
                    # ABS: a name the linker defines, or in an overlay's ELF
                    # a main-executable name
                    self.addr.setdefault(name, value)
        objs.sort(key=lambda o: (o.addr, -o.size, o.local))
        # One object per address (the biggest, a global before a local)
        self.objs = []
        for o in objs:
            if self.objs and self.objs[-1].addr == o.addr:
                continue
            self.objs.append(o)
        self.starts = [o.addr for o in self.objs]
        self.by_addr = {o.addr: o for o in self.objs}
        self.by_name = {o.name: o for o in self.objs if not o.local}

    def binary_of(self, addr):
        for lo, hi, b in self.ranges:
            if lo <= addr < hi:
                return b
        return None

    def containing(self, addr):
        """The object addr is inside, or None"""
        i = bisect.bisect_right(self.starts, addr) - 1
        # an object may sit inside a bigger one before it (a label in an
        # asm table): look back a little
        for j in range(i, max(i - 8, -1), -1):
            o = self.objs[j]
            if o.addr <= addr < o.end:
                return o
        return None

    def span(self, lo, hi):
        """The objects overlapping [lo, hi), in address order (nested ones
        left out)"""
        i = max(bisect.bisect_right(self.starts, lo) - 1, 0)
        out = []
        while i < len(self.objs) and self.objs[i].addr < hi:
            o = self.objs[i]
            if o.end > lo and not o.func and not (out and o.end <= out[-1].end):
                out.append(o)
            i += 1
        return out

    def home(self, name, addr, aliases):
        """The object an access through name is meant to be in: for a name
        the linker defines relative to another (`D_x = base + off`, maybe
        a biased base outside it), the base's object"""
        seen = 0
        while name in aliases and name not in self.by_name and seen < 8:
            name = aliases[name][0]
            seen += 1
        a = self.addr.get(name, addr) if seen else addr
        return self.containing(a) if a is not None else None

    def resolve(self, name, binary=None, file=None):
        """A name's original address: a global, a linker-defined name, or a
        local (unique, or unique in that binary and file); None if unknown"""
        if name in self.addr:
            return self.addr[name]
        ls = self.locals.get(name)
        if not ls:
            return None
        if len(ls) > 1 and binary:
            ls = [o for o in ls if o.binary == binary] or ls
        if len(ls) > 1 and file:
            base = os.path.basename(file)
            ls = [o for o in ls if o.file and os.path.basename(o.file) == base] or ls
        return ls[0].addr if len(ls) == 1 else None


# ---------------------------------------------------------------------------
# The port's layout (lld's map)


def port_map(path):
    """{name: address} from lld's -Map output, or {} if there is none"""
    out = {}
    if not path or not os.path.exists(path):
        return out
    with open(path, encoding="latin-1") as f:
        for line in f:
            m = re.match(r"([0-9a-f]{8}) 00000000\s+0\s+(\S+)$", line)
            if m:
                n = m.group(2)
                out.setdefault(n[len(syms.PREFIX):] if n.startswith(syms.PREFIX) else n, int(m.group(1), 16))
    return out


# ---------------------------------------------------------------------------
# One translation unit

_TMP = None
_CLANG = None


def _init(tmp, clang):
    global _TMP, _CLANG
    _TMP, _CLANG = tmp, clang


PREFIX = re.compile(r"[ |`-]*")
LOC = re.compile(r"<invalid sloc>|line:(\d+):\d+|col:\d+|<(?:built-in|scratch space|command line)>:(\d+):\d+"
                 r"|([^\s<>,'=]+):(\d+):\d+(?=[\s>,]|$)")
SKIP_SUBTREE = {"RecordDecl", "EnumDecl", "TypedefDecl", "FieldDecl", "IndirectFieldDecl", "ParmVarDecl"}
STRIP = {"ImplicitCastExpr", "ParenExpr"}


def _types(line, q):
    """The (sugared, desugared) type quoted at q"""
    e = line.index("'", q + 1)
    t = line[q + 1:e]
    if line.startswith(":'", e + 1):
        e2 = line.index("'", e + 3)
        return t, line[e + 3:e2]
    return t, t


def norm_path(p):
    p = os.path.normpath(p.replace("\\", "/")).replace("\\", "/")
    if os.path.isabs(p):
        try:
            r = os.path.relpath(p, ROOT).replace("\\", "/")
            if not r.startswith(".."):
                return r
        except ValueError:
            pass
    return p


class Node:
    __slots__ = ("kind", "text", "kids", "loc", "q")

    def __init__(self, kind, text, loc, q):
        self.kind, self.text, self.loc, self.q, self.kids = kind, text, loc, q, []

    def type(self, desugared=True):
        if self.q < 0:
            return None
        s, d = _types(self.text, self.q)
        return d if desugared else s


def parse_tree(text):
    """The text AST dump as a tree of Nodes (declarations of types and
    parameters left out), each with its (file, line)"""
    root = Node("TranslationUnitDecl", "", None, -1)
    stack = [root]
    cur_file, cur_line = "?", 0
    skip_depth = None
    for raw in text.splitlines():
        p = PREFIX.match(raw).end()
        if p >= len(raw):
            continue
        depth = p // 2
        sp = raw.find(" ", p)
        kind = raw[p:sp] if sp > 0 else raw[p:]
        q = raw.find("'", p)
        head = raw[p:q] if q >= 0 else raw[p:]
        first = None
        if ":" in head:
            for m in LOC.finditer(head):
                g = m.group(0)
                if g.startswith("line:"):
                    cur_line = int(m.group(1))
                elif g.startswith("col:") or g == "<invalid sloc>":
                    pass
                elif m.group(2) is not None:
                    cur_file, cur_line = g[:g.index(">") + 1], int(m.group(2))
                else:
                    cur_file, cur_line = m.group(3), int(m.group(4))
                if first is None and g != "<invalid sloc>":
                    first = (cur_file, cur_line)
        if skip_depth is not None:
            if depth > skip_depth:
                continue
            skip_depth = None
        if kind in SKIP_SUBTREE:
            skip_depth = depth
            continue
        if depth == 0:
            continue
        n = Node(kind, raw[p:], first or (cur_file, cur_line), q - p if q >= 0 else -1)
        del stack[depth:]
        if not stack:
            continue
        stack[-1].kids.append(n)
        stack.append(n)
    return root


VAR_DECL = re.compile(r"VarDecl (0x[0-9a-f]+)(?: prev (0x[0-9a-f]+))? <[^>]*> (?:[^ ]+ )?"
                      r"(?:(used|referenced) )?(\S+) '")
VAR_REF = re.compile(r" lvalue Var (0x[0-9a-f]+) '([^']+)'")
INT_LIT = re.compile(r"IntegerLiteral 0x[0-9a-f]+ <[^>]*> '[^']*' (-?\d+)")


def strip(n):
    while n.kind in STRIP and n.kids:
        n = n.kids[0]
    return n


def address_of(n, gvars):
    """(var addr, through a cast) if n is the address of a global variable:
    &x, an array decaying, a cast of either"""
    n = strip(n)
    cast = False
    while n.kind == "CStyleCastExpr" and n.kids:
        n = strip(n.kids[0])
        cast = True
    if n.kind == "UnaryOperator" and "'&'" in n.text and n.kids:
        r = strip(n.kids[0])
        if r.kind == "DeclRefExpr":
            m = VAR_REF.search(r.text)
            if m and m.group(1) in gvars:
                return m.group(1), cast
        return None
    if n.kind == "DeclRefExpr":
        m = VAR_REF.search(n.text)
        if m and m.group(1) in gvars and "[" in (n.type() or ""):
            return m.group(1), cast
    return None


def analyse_tree(root):
    """(declarations, casts, calls, referenced decl addresses) of one TU"""
    gvars = {}       # addr -> decl dict (one per redeclaration)
    canon = {}       # addr -> first decl's addr
    casts, calls, refs = [], [], set()
    stack = [(root, 0, None)]

    def visit(n, depth, fn):
        if n.kind == "VarDecl":
            m = VAR_DECL.match(n.text)
            if m:
                rest = n.text[n.q:]
                file_scope = depth == 1
                ext = bool(re.search(r"\sextern(\s|$)", rest))
                if file_scope or ext:
                    addr, prev = m.group(1), m.group(2)
                    canon[addr] = canon.get(prev, prev) if prev else addr
                    gvars[addr] = {"name": m.group(4), "type": n.type(False), "dtype": n.type(),
                                   "extern": ext and "cinit" not in rest,
                                   "static": bool(re.search(r"\sstatic(\s|$)", rest)), "file_scope": file_scope,
                                   "loc": n.loc, "fn": fn}
        elif n.kind == "DeclRefExpr":
            m = VAR_REF.search(n.text)
            if m:
                refs.add(m.group(1))
        elif n.kind == "CStyleCastExpr" and n.kids:
            t = n.type()
            a = address_of(n.kids[0], gvars)
            if a and t and t.endswith("*"):
                casts.append({"var": a[0], "to": n.type(False), "dto": t, "loc": n.loc, "fn": fn})
        elif n.kind == "CallExpr" and len(n.kids) > 1:
            callee = strip(n.kids[0])
            cname = None
            m = re.search(r" Function 0x[0-9a-f]+ '([^']+)'", callee.text)
            if m:
                cname = m.group(1)
            vars_, consts = [], []
            for k in n.kids[1:]:
                a = address_of(k, gvars)
                if a:
                    vars_.append(a[0])
                    continue
                s = strip(k)
                if s.kind == "IntegerLiteral":
                    lm = INT_LIT.match(s.text)
                    if lm:
                        consts.append(("int", int(lm.group(1))))
                elif s.kind == "UnaryExprOrTypeTraitExpr" and " sizeof " in s.text:
                    tq = s.text.find("'", s.text.find(" sizeof ") + 1)
                    if tq >= 0 and not s.kids:
                        sg, ds = _types(s.text, tq)
                        consts.append(("sizeof", sg, ds))
                    elif s.kids:
                        r = strip(s.kids[0])
                        if r.kind == "DeclRefExpr":
                            vm = VAR_REF.search(r.text)
                            if vm:
                                consts.append(("sizeofvar", vm.group(1)))
            if vars_ and consts:
                calls.append({"callee": cname, "vars": vars_, "consts": consts, "loc": n.loc, "fn": fn})

    while stack:
        n, depth, fn = stack.pop()
        visit(n, depth, fn)
        if n.kind == "FunctionDecl" and depth == 1:
            m = re.search(r"\s(\w+) '", n.text)
            fn = m.group(1) if m else fn
        for k in reversed(n.kids):
            stack.append((k, depth + 1, fn))
    return gvars, canon, casts, calls, refs


def spellable(t):
    return t and "unnamed" not in t and "anonymous" not in t and " at " not in t


def probe_sizes(src, flags, exprs):
    """{index: sizeof(expr)} for each expression that compiles at the end of
    src; those that don't (an incomplete type) are dropped"""
    with open(src, encoding="latin-1") as f:
        text = f.read()
    probe = src[:-2] + "_oa.c"
    live = dict(enumerate(exprs))
    out = {}
    for _ in range(4):
        if not live:
            break
        order = sorted(live)
        body = '\n#line 1 "__grx_oa_probe"\n' + "".join(
            f"typedef char __grx_oa_{k}[sizeof({live[k]})];\n" for k in order)
        with open(probe, "w", encoding="latin-1", newline="\n") as f:
            f.write(text + body)
        p = subprocess.run([_CLANG, "-target", build.TARGET, "-fsyntax-only", probe] + flags
                           + ["-ferror-limit=0", "-Xclang", "-ast-dump", "-Xclang", "-ast-dump-filter=__grx_oa_",
                              "-fno-color-diagnostics"],
                           capture_output=True, cwd=ROOT)
        bad = {int(m.group(1)) - 1 for m in re.finditer(r"__grx_oa_probe:(\d+):\d+: error", p.stderr.decode("latin-1"))}
        if p.returncode and not bad:
            break
        if not bad:
            for m in re.finditer(r"__grx_oa_(\d+) 'char\[(\d+)\]'", p.stdout.decode("latin-1")):
                out[int(m.group(1))] = int(m.group(2))
            break
        for i in bad:
            if 0 <= i < len(order):
                live.pop(order[i], None)
    try:
        os.remove(probe)
    except OSError:
        pass
    return out


ASM_SYM = re.compile(r"(?<![\w.$@])(\$?)(_[A-Za-z_][\w.]*)((?:[+-]\d+)*)(\()?")
SECTION = re.compile(r"\.(text|data|bss)\b|\.section\s+([^,\s]+)")


def access_width(insn, immediate):
    """How many bytes an instruction reads or writes at its memory operand
    (0 for an address computed, not accessed)"""
    op = insn.split(None, 1)[0]
    if immediate or op.startswith("lea"):
        return 0
    if re.match(r"(v?mov(ups|aps|dqu|dqa|upd|apd)|[a-z]*p[sd]$|p[a-z]+)", op) and "ss" not in op[-2:] \
            and not op.startswith(("push", "pop")):
        return 16
    if op in ("movsd", "movq", "movlps", "movhps", "movlpd", "movhpd", "fldl", "fstpl", "fstl", "cvtsd2ss",
              "addsd", "subsd", "mulsd", "divsd", "ucomisd", "comisd", "fildll", "fistpll"):
        return 8
    if op.endswith("ss") or op in ("flds", "fsts", "fstps", "cvtss2sd"):
        return 4
    if re.match(r"mov[sz]b", op):
        return 1
    if re.match(r"mov[sz]w", op):
        return 2
    return {"b": 1, "w": 2, "l": 4, "q": 8}.get(op[-1], 4)


def scan_asm(text):
    """[(name, offset, indexed, kind, function or data label, (file, line), insn)]:
    every reference to a symbol with an offset in the assembly"""
    files = {}
    cur = None
    sec = "code"
    label = None
    out = []
    for line in text.splitlines():
        s = line.strip()
        if not s or s.startswith("#"):
            continue
        if s.startswith(".file"):
            m = re.match(r'\.file\s+(\d+)\s+"([^"]*)"(?:\s+"([^"]*)")?', s)
            if m:
                files[m.group(1)] = m.group(3) if m.group(3) is not None else m.group(2)
            continue
        if s.startswith(".loc"):
            m = re.match(r"\.loc\s+(\d+)\s+(\d+)", s)
            if m:
                cur = (norm_path(files.get(m.group(1), "?")), int(m.group(2)))
            continue
        m = SECTION.match(s)
        if m:
            n = m.group(1) or m.group(2).strip('"')
            n = n.lstrip(".").split("$")[0]
            sec = "code" if n == "text" else "data" if n in ("data", "rdata", "bss") else "other"
            continue
        if sec == "other":
            continue
        m = re.match(r"(_[\w.]+):", s)
        if m:
            label = m.group(1)[1:]
            if sec == "data":
                cur = None
            continue
        if s.startswith(".") and not s.startswith((".long", ".quad", ".int")):
            continue
        if s.startswith(("call", "jmp", "j")) and sec == "code":
            continue
        s = s.split("#", 1)[0].strip()
        for m in ASM_SYM.finditer(s):
            name = m.group(2)[1:]
            off = sum(int(x) for x in re.findall(r"[+-]\d+", m.group(3))) if m.group(3) else 0
            indexed = bool(m.group(4)) and not re.match(r"\(\s*\)", s[m.end() - 1:])
            width = access_width(s, m.group(1) == "$") if sec == "code" else 4
            out.append((name, off, indexed, sec, label, cur, s, width))
    return out


def audit_one(job):
    """One translation unit -> dict of plain data"""
    rel, binary = job
    res = {"rel": rel, "binary": binary, "err": None, "decls": [], "casts": [], "calls": [], "warn": [],
           "access": []}
    try:
        src = build.generate(rel, out_dir=_TMP)
    except Exception as e:  # noqa: BLE001
        res["err"] = f"generate: {e}"
        return res
    flags = [f for f in build.game_flags(rel, "1") if f != "-w"]
    # names this file defines as aliases (port/tools/fixups/objects.py)
    with open(src, encoding="latin-1") as f:
        res["aliases"] = re.findall(r"\.set " + syms.PREFIX + r"(\w+), ", f.read())
    # 1. the code, with extern data addressed directly
    asm = src[:-2] + ".s"
    p = subprocess.run([_CLANG, "-target", build.TARGET, "-S", "-o", asm, src] + flags
                       + ["-w", "-fno-auto-import"], capture_output=True, cwd=ROOT)
    if p.returncode:
        res["err"] = "compile: " + p.stderr.decode("latin-1")[-1500:]
    else:
        with open(asm, encoding="latin-1") as f:
            res["access"] = [a for a in scan_asm(f.read()) if a[1] or a[7] > 4]
        os.remove(asm)
    # 2. the AST, and the array-bounds warnings
    p = subprocess.run([_CLANG, "-target", build.TARGET, "-fsyntax-only", src] + flags
                       + ["-Wno-everything", "-Warray-bounds", "-Warray-bounds-pointer-arithmetic",
                          "-Xclang", "-ast-dump", "-fno-color-diagnostics"],
                       capture_output=True, cwd=ROOT)
    for m in re.finditer(r"^(.*?):(\d+):\d+: warning: (.*\[-Warray-bounds[^\]]*\])\r?$",
                         p.stderr.decode("latin-1"), re.M):
        res["warn"].append((norm_path(m.group(1)), int(m.group(2)), m.group(3)))
    tree = parse_tree(p.stdout.decode("latin-1"))
    gvars, canon, casts, calls, refs = analyse_tree(tree)
    del tree
    used = {canon.get(a, a) for a in refs}
    # 3. sizes: each name as the file sees it at its end, each block-scope
    # extern's type, and the types of the casts and sizeofs
    exprs, keys = [], []

    def want(key, e):
        if spellable(e):
            keys.append(key)
            exprs.append(e)

    names_seen = set()
    for a, d in gvars.items():
        if d["file_scope"] and d["name"] not in names_seen:
            names_seen.add(d["name"])
            want(("var", d["name"]), d["name"])
        elif not d["file_scope"]:
            want(("type", d["type"]), d["type"])
            want(("type", d["dtype"]), d["dtype"])
    for c in casts:
        for t in (c["to"], c["dto"]):
            if t.endswith("*"):
                want(("type", t[:-1].strip()), t[:-1].strip())
    for c in calls:
        for k in c["consts"]:
            if k[0] == "sizeof":
                want(("type", k[1]), k[1])
                want(("type", k[2]), k[2])
    uniq = {}
    for k, e in zip(keys, exprs):
        uniq.setdefault(k, e)
    keys = list(uniq)
    sizes = probe_sizes(src, flags + ["-w"], [uniq[k] for k in keys]) if keys else {}
    size_of = {keys[i]: v for i, v in sizes.items()}

    def tsize(*ts):
        for t in ts:
            if t and ("type", t) in size_of:
                return size_of[("type", t)]
        return None

    for a, d in gvars.items():
        if d["file_scope"]:
            sz = size_of.get(("var", d["name"]))
        else:
            sz = tsize(d["type"], d["dtype"])
        res["decls"].append({"name": d["name"], "type": d["type"], "size": sz, "extern": d["extern"],
                             "static": d["static"], "used": canon.get(a, a) in used,
                             "file": norm_path(d["loc"][0]), "line": d["loc"][1], "fn": d["fn"]})
    for c in casts:
        d = gvars.get(c["var"])
        t = c["to"][:-1].strip()
        res["casts"].append({"name": d["name"], "static": d["static"], "to": c["to"],
                             "size": tsize(t, c["dto"][:-1].strip()),
                             "file": norm_path(c["loc"][0]), "line": c["loc"][1], "fn": c["fn"]})
    for c in calls:
        consts = []
        for k in c["consts"]:
            if k[0] == "int":
                consts.append((str(k[1]), k[1]))
            elif k[0] == "sizeof":
                consts.append((f"sizeof({k[1]})", tsize(k[1], k[2])))
            elif k[0] == "sizeofvar" and k[1] in gvars:
                nm = gvars[k[1]]["name"]
                consts.append((f"sizeof {nm}", size_of.get(("var", nm))))
        res["calls"].append({"callee": c["callee"], "names": [gvars[v]["name"] for v in c["vars"]],
                             "static": [gvars[v]["static"] for v in c["vars"]], "consts": consts,
                             "file": norm_path(c["loc"][0]), "line": c["loc"][1], "fn": c["fn"]})
    return res


# ---------------------------------------------------------------------------
# Putting it together

# Callees whose constant argument next to an address is a byte count (a
# sizeof() argument always counts); a texture or packet call's constants
# are GS addresses and sizes in pixels
SIZE_CALLEES = re.compile(r"(?i)^(?!.*(packet|tex|env|draw)).*(mem|cpy|copy|clear|zero|fill|savedata|reqread|reqwrite"
                          r"|writeback|flush|dcache|rpc|send|recv|^read$|^write$)")


class Case:
    def __init__(self, kind, home, lo, hi):
        self.kind, self.home, self.lo, self.hi = kind, home, lo, hi
        self.sites = []



def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count())
    ap.add_argument("-o", "--output", default=REPORT)
    ap.add_argument("--map", default=os.path.join(ROOT, "build", "port", "grandia.map"),
                    help="the port's linker map, to say whether its layout happens to keep each case's distances")
    ap.add_argument("--cache", metavar="FILE",
                    help="keep the per-file results in FILE (pickle) and reuse them: for working on the report")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("files", nargs="*", help="only these sources (for testing)")
    args = ap.parse_args()
    t0 = time.time()

    layout = Layout(ROOT)
    pmap = port_map(args.map)
    try:
        import importlib.util
        spec = importlib.util.spec_from_file_location("fixups_objects",
                                                      os.path.join(os.path.dirname(__file__), "fixups", "objects.py"))
        objmod = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(objmod)
        blocks = getattr(objmod, "BLOCKS", [])
    except FileNotFoundError:
        blocks = []

    jobs = [(rel, b) for rel, b, host in build.game_sources() if not host]
    if args.files:
        want = {os.path.relpath(os.path.abspath(f), ROOT).replace("\\", "/") for f in args.files}
        jobs = [j for j in jobs if j[0] in want]
    if args.cache and os.path.exists(args.cache):
        import pickle
        with open(args.cache, "rb") as f:
            results = pickle.load(f)
        report(args, layout, pmap, blocks, results, t0)
        return
    clang = build.find_clang()
    tmp = tempfile.mkdtemp(prefix="object_audit_")
    results = []
    try:
        with multiprocessing.Pool(args.jobs, initializer=_init, initargs=(tmp, clang)) as pool:
            for n, r in enumerate(pool.imap_unordered(audit_one, jobs, chunksize=2)):
                results.append(r)
                if args.verbose and n % 250 == 0:
                    print(f"{n}/{len(jobs)} {time.time() - t0:.0f}s", file=sys.stderr)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    results.sort(key=lambda r: r["rel"])
    if args.cache:
        import pickle
        with open(args.cache, "wb") as f:
            pickle.dump(results, f)
    report(args, layout, pmap, blocks, results, t0)


def report(args, layout, pmap, blocks, results, t0):
    ln = syms.LinkerNames(ROOT)
    aliases = {n: (b, o) for n, (b, o, _) in ln.rel.items()}
    biased = {n for n, (b, o, binary) in ln.rel.items() if binary == "any"}
    # Which port source defines each name
    def_src = {}
    for r in results:
        for n in r.get("aliases", ()):
            def_src.setdefault(n, r["rel"] + " (an alias in a block)")
        for d in r["decls"]:
            if not d["extern"] and d["fn"] is None and not d["static"]:
                def_src.setdefault(d["name"], r["rel"])

    def where(o):
        if o.local:
            return f"static, {o.file}" if o.file else "static"
        return def_src.get(o.name) or "(not in C: stubs.c or linker_data.c)"

    def in_block(lo, hi):
        for b in blocks:
            if b["lo"] <= lo and hi <= b["hi"]:
                return b
        return None

    cases = {}

    def case(kind, home, lo, hi, site):
        k = (kind, home.addr)
        c = cases.get(k)
        if c is None:
            c = cases[k] = Case(kind, home, min(lo, home.addr), max(hi, home.end))
        c.lo, c.hi = min(c.lo, lo), max(c.hi, hi)
        if site not in c.sites:
            c.sites.append(site)

    def home_of(name, binary, rel):
        a = layout.resolve(name, binary, rel)
        if a is None:
            return None, None
        h = layout.home(name, a, aliases)
        return a, (h if h is not None and not h.func else None)

    # (b), (d): offsets in the code and data
    loose = []          # (kind "edge"/"indexed", target, home, text)
    unresolved = collections.Counter()
    for r in results:
        for name, off, idx, sec, label, loc, insn, width in r["access"]:
            a, home = home_of(name, r["binary"], r["rel"])
            if a is None:
                if name in layout.locals:
                    unresolved[name] += 1
                continue
            if home is None:
                continue
            t = a + off
            if home.addr <= t and (t + width <= home.end if width else t < home.end):
                continue
            at = f"{loc[0]}:{loc[1]}" if loc else r["rel"]
            where_ = f"{at} in {label}" if sec == "code" else f"{r['rel']}, data {label}"
            text = f"{name}{off:+#x} = 0x{t:08X}  {where_}: {insn}"
            if idx:
                loose.append(("indexed", t, home, text))
            elif t == home.end and not width:
                loose.append(("edge", t, home, text))
            else:
                case("d" if sec == "data" or name in aliases else "b", home, t, t + 1, text)

    # (a): declarations bigger than the definition, as each file sees them
    big = collections.defaultdict(list)
    for r in results:
        for d in r["decls"]:
            if d["size"] is None or not d["used"] or d["static"]:
                continue
            a, home = home_of(d["name"], r["binary"], r["rel"])
            if home is not None and a + d["size"] > home.end:
                big[(d["name"], d["type"], d["size"])].append((r["rel"], d))
    for (name, typ, size), uses in sorted(big.items()):
        a, home = home_of(name, None, None)
        tus = sorted({rel for rel, _ in uses})
        decl_at = sorted({f"{d['file']}:{d['line']}" for _, d in uses})
        case("a", home, a, a + size,
             f"'{typ}' (0x{size:X} bytes) declared at {', '.join(decl_at[:4])}; used in {len(tus)} files: "
             + ", ".join(tus[:10]) + (" ..." if len(tus) > 10 else ""))
    for r in results:
        # (c): casts of an object's address to a pointer to something bigger
        for c in r["casts"]:
            if c["size"] is None or c["static"]:
                continue
            a, home = home_of(c["name"], r["binary"], r["rel"])
            if home is not None and a + c["size"] > home.end:
                case("c", home, a, a + c["size"],
                     f"{c['file']}:{c['line']} in {c['fn']}: ({c['to']})&{c['name']}, 0x{c['size']:X} bytes")
        # (d): an address passed with a size bigger than the object
        for c in r["calls"]:
            for name, st in zip(c["names"], c["static"]):
                if st:
                    continue
                a, home = home_of(name, r["binary"], r["rel"])
                if home is None:
                    continue
                for text, v in c["consts"]:
                    if v is None or v <= home.end - a or v > 0x40000:
                        continue
                    if not text.startswith("sizeof") and not SIZE_CALLEES.search(c["callee"] or ""):
                        continue
                    case("d", home, a, a + v,
                         f"{c['file']}:{c['line']} in {c['fn']}: {c['callee']}(&{name}, ..., {text} = 0x{v:X})")
    # (d): names the linker defines outside the object they're relative to
    biased_out = []
    for name, (base, off) in sorted(aliases.items()):
        if name.startswith(".") or base.startswith("__rodata_"):
            continue
        b = layout.resolve(base)
        home = layout.containing(b) if b is not None else None
        if home is None or home.func:
            continue
        t = b + off
        if home.addr <= t < home.end:
            continue
        if name in biased:
            biased_out.append((name, base, off, t, home))
        else:
            case("d", home, t, t + 1, f"linker-defined: {name} = {base} + 0x{off:X} = 0x{t:08X} (build/c_syms*.ld)")

    # Group the cases by the objects they involve; edge and indexed
    # accesses inside a group join it
    spans = []
    for c in sorted(cases.values(), key=lambda c: (c.lo, c.hi)):
        if spans and c.lo < spans[-1][1]:
            spans[-1][1] = max(spans[-1][1], c.hi)
            spans[-1][2].append(c)
        else:
            spans.append([c.lo, c.hi, [c]])
    leftover = []
    for kind, t, home, text in loose:
        for sp in spans:
            if sp[0] <= home.addr < sp[1] and sp[0] <= t < sp[1]:
                sp[2].append(Case(kind, home, t, t))
                sp[2][-1].sites.append(text)
                break
        else:
            leftover.append((kind, t, home, text))

    out = []
    w = out.append
    by_kind = collections.Counter(c.kind for c in cases.values())
    w("Objects that rely on their neighbours (port/tools/object_audit.py; port/README.md)")
    w("")
    w("Original layout: the matching build's ELFs. Port layout: " + os.path.relpath(args.map, ROOT).replace("\\", "/")
      + ("" if pmap else " (missing)") + ". Fixes: port/tools/fixups/objects.py.")
    w("Kinds: a declared bigger than defined; b constant offset past the end; c cast to a type spanning")
    w("several objects; d address plus an offset beyond the object (data tables, sizes passed with it,")
    w("linker-defined names); edge = exactly at the end; indexed = a base outside it plus an index register.")
    w("")
    n_fixed = sum(1 for lo, hi, _ in spans if in_block(lo, hi))
    w(f"{len(spans)} groups of objects ({n_fixed} made one block by objects.py); cases by kind: "
      + ", ".join(f"{k} {by_kind[k]}" for k in "abcd") + f"; {len(leftover)} edge/indexed accesses outside any group.")
    errs = [r for r in results if r["err"]]
    if errs:
        w(f"{len(errs)} sources failed: " + ", ".join(r["rel"] for r in errs[:20]))
    w("")
    order = {"a": 0, "b": 1, "c": 2, "d": 3, "edge": 4, "indexed": 5}
    for lo, hi, cl in spans:
        span = layout.span(lo, hi)
        kinds = "".join(sorted({c.kind for c in cl if len(c.kind) == 1}))
        names = f"{span[0].name} .. {span[-1].name}" if len(span) > 1 else (span[0].name if span else "?")
        w("=" * 100)
        w(f"[{kinds}] {names}  ({layout.binary_of(lo)} 0x{lo:08X}-0x{hi:08X}, {len(span)} objects)")
        blk = in_block(lo, hi)
        if blk:
            w(f"  fixed: one block, {blk['name']} (port/tools/fixups/objects.py)")
        else:
            glob_ = [o for o in span if not o.local and o.name in pmap]
            if pmap and len(glob_) > 1:
                same = all(pmap[o.name] - pmap[glob_[0].name] == o.addr - glob_[0].addr for o in glob_)
                w("  NOT FIXED; the port's layout " + ("happens to keep these distances" if same else "differs"))
            else:
                w("  NOT FIXED")
        if not any(where(o) and not where(o).startswith("(") for o in span):
            w("  (none of these is defined by the port's game C: library code the host C runtime"
              " replaces, or stubs)")
        w("  objects (original order):")
        for o in span[:60]:
            w(f"    0x{o.addr:08X} {o.size:#8x}  {o.name:<34} {where(o)}")
        if len(span) > 60:
            w(f"    ... {len(span) - 60} more")
        merged = collections.OrderedDict()
        for c in sorted(cl, key=lambda c: (order[c.kind], c.home.addr)):
            merged.setdefault((c.kind, c.home.name), []).extend(c.sites)
        for (kind, hname), sites in merged.items():
            w(f"  {kind} (from {hname}):")
            for s in sites[:30]:
                w(f"      {s}")
            if len(sites) > 30:
                w(f"      ... {len(sites) - 30} more")
    w("")
    w("=" * 100)
    w("Linker-defined biased bases (config/symbol_exprs.ld): outside their table by design, the code's")
    w("index brings them back in; accesses through them are checked against the table:")
    for name, base, off, t, home in biased_out:
        w(f"  {name} = {base} {off:+#x} (0x{t:08X}; table 0x{home.addr:08X}, 0x{home.size:X} bytes)")
    w("")
    by_home = collections.defaultdict(list)
    in_blocks = 0
    for kind, t, home, text in leftover:
        if in_block(min(t, home.addr), max(t + 1, home.end)):
            in_blocks += 1
            continue
        if kind == "indexed":
            kind = "indexed, base before" if t < home.addr else "indexed, base past the end"
        by_home[home.addr].append((kind, text))
    w("Edge and indexed accesses outside the groups above, and outside the blocks objects.py makes")
    w(f"({in_blocks} more are inside those blocks). Usually harmless: an end pointer (a loop's bound), a")
    w("biased base (a[id - 0x1001]), or clang's loop over an array that counts a negative index up to")
    w("zero from the end (base = the array's end). Harmful only if the index doesn't bring it back:")
    for a, l in sorted(by_home.items()):
        o = layout.by_addr[a]
        nxt = layout.containing(o.end)
        w(f"  {o.name} (0x{o.addr:08X}, 0x{o.size:X}; next {nxt.name if nxt else '-'}; {where(o)})")
        for kind, text in l[:4]:
            w(f"      {kind}: {text}")
        if len(l) > 4:
            w(f"      ... {len(l) - 4} more")
    w("")
    w("clang -Warray-bounds (the file's own view of the type):")
    seen = set()
    for r in results:
        for f, line, msg in r["warn"]:
            if (f, line, msg) not in seen:
                seen.add((f, line, msg))
                w(f"  {f}:{line}: {msg}")
    if unresolved:
        w("")
        w("Statics not resolved to one original object: "
          + ", ".join(f"{n} ({k})" for n, k in unresolved.most_common(60)))
    w("")
    w(f"({len(results)} sources, {time.time() - t0:.0f}s)")
    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    with open(args.output, "w", newline="\n") as f:
        f.write("\n".join(out) + "\n")
    print(f"{os.path.relpath(args.output, ROOT)}: {len(spans)} groups ({n_fixed} fixed); "
          + ", ".join(f"{k} {by_kind[k]}" for k in "abcd"))


if __name__ == "__main__":
    main()
