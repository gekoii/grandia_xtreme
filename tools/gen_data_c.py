#!/usr/bin/env python3
"""Draft a data-only C file from an asm data piece (plan step 6b, see
docs/shifting.md and docs/notes/data_step6b_rules.md).

Usage: gen_data_c.py <binary> <start> <end> [-o out.c]

<start>/<end> are vram addresses of a range of the binary's .data or
.rodata that is still asm (one or more asm pieces of splat's split). The
draft has one object per label, in address order:

  - a string (with its padding) -> char NAME[] = "...";
  - words -> u32 NAME[] (relocated words as (u32)&sym + off), or a pointer
    table (void *NAME[] / Func NAME[]) when every word is a pointer or 0;
  - halfwords / bytes / floats -> u16 / u8 / f32 arrays; one element -> a scalar;
  - anything mixed -> a struct with one field per run.

gcc 2.9 aligns every array, struct and string object to 8 (DATA_ALIGNMENT),
so a label where no such object can start (4 mod 8, odd, ...) becomes a
field of the object before it: its name is listed in that object's comment
and tools/c_syms.py defines it relative to the object. Objects in .rodata
are const. Symbols the data refers to are declared extern locally (no
project headers). The draft compiles and should pass tools/check_data.py
as it is; what's left for a person is the types (real structs, string
tables, merged fields), see docs/notes/data_step6b_rules.md.
"""
import argparse
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from data_asm import (Symbols, asm_pieces, binary_for_data, natural_align,  # noqa: E402
                      parse_expr, rom)
from binaries import BINARIES  # noqa: E402

TYPES = {"u32": 4, "u16": 2, "u8": 1, "f32": 4, "str": 1, "pad": 1}


def c_string(data):
    """C string literal for bytes (without the final NUL); octal escapes so a
    following digit can't extend them."""
    out = []
    for i, c in enumerate(data):
        ch = chr(c)
        if ch == '"' or ch == "\\":
            out.append("\\" + ch)
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\t":
            out.append("\\t")
        elif 0x20 <= c < 0x7F:
            # '??x' would be a trigraph
            if ch == "?" and i + 1 < len(data) and data[i + 1] == ord("?"):
                out.append("\\077")
            else:
                out.append(ch)
        else:
            out.append("\\%03o" % c)
    return '"' + "".join(out) + '"'


def f32_literal(word):
    v = struct.unpack("<f", struct.pack("<I", word))[0]
    if v != v or v in (float("inf"), float("-inf")):
        return None
    for digits in (6, 7, 8, 9):
        s = "%.*g" % (digits, v)
        if struct.unpack("<I", struct.pack("<f", float(s)))[0] == word:
            if "e" not in s and "." not in s:
                s += ".0"
            return s + "f"
    return None


def natural_align_of(addr):
    return natural_align(addr)


def font_text(data):
    """The text of a string in the game's font code (each character is its
    ASCII code + 0x1F followed by 0xF9; a space is 0x9F), as a quoted
    string, or None. Digits follow the same rule ('0' is 0x4F): the BATTLE
    tactic caption reads "Safety 1st", the list pages "Item 1".."Item 5"."""
    if len(data) < 2 or len(data) % 2 or any(data[i + 1] != 0xF9 for i in range(0, len(data), 2)):
        return None
    out = []
    for i in range(0, len(data), 2):
        c = data[i]
        if c == 0x9F:
            out.append(" ")
        elif 0x21 + 0x1F <= c <= 0x7E + 0x1F and chr(c - 0x1F) not in '"\\':
            out.append(chr(c - 0x1F))  # printable ASCII: letters, digits, punctuation
        else:
            out.append("?")
    return '"' + "".join(out) + '"'


class Elem:
    """One element of an object: kind u32/u16/u8/f32/str/pad, its address, size,
    value (int, or bytes for str/pad) and relocation (symbol, addend) for u32."""

    def __init__(self, addr, kind, size, value, reloc=None):
        self.addr, self.kind, self.size, self.value, self.reloc = addr, kind, size, value, reloc

    @property
    def end(self):
        return self.addr + self.size


class Obj:
    def __init__(self, name, addr):
        self.name, self.addr = name, addr
        self.elems = []
        self.fields = []  # (name, offset): labels inside the object
        self.aliases = []
        self.save = False
        self.end = addr
        self.ctype = None


def elements(b, start, end, image, syms):
    """Elements of [start, end) from the asm pieces (types and relocations) and
    the original bytes (values). Bytes no directive covers are padding."""
    pieces = asm_pieces(b, start, end)
    pos = start
    for p in pieces:
        if p.start > pos:
            break
        pos = max(pos, p.end)
    if pos < end:
        raise SystemExit(f"0x{start:08X}-0x{end:08X}: 0x{pos:08X} is not in an asm data piece "
                         f"(already C, or a C file's .rodata); choose a range inside asm pieces")
    sections = {p.section for p in pieces}
    if len(sections) != 1:
        raise SystemExit(f"0x{start:08X}-0x{end:08X} spans {sorted(sections)}")
    vram = BINARIES[b]["vram"]
    labels, items = {}, []
    for p in pieces:
        for v, names in p.labels.items():
            if start <= v < end:
                labels.setdefault(v, []).extend(n for n in names if n not in labels.get(v, []))
        items += [it for it in p.items if start <= it[0] < end]
    items.sort()
    out = []
    pos = start
    for v, kind, op in items:
        if v > pos:
            gap = image[pos - vram:v - vram]
            out.append(Elem(pos, "pad" if not any(gap) else "u8", len(gap), gap))
            if any(gap):
                out[-1:] = [Elem(pos + i, "u8", 1, c) for i, c in enumerate(gap)]
        if kind == "asciz":
            raw = image[v - vram:end - vram]
            n = raw.index(b"\0") + 1
            # splat writes zero words after a string as empty strings: padding
            out.append(Elem(v, "str", n, raw[:n]) if n > 1 else Elem(v, "pad", 1, raw[:1]))
            pos = v + n
            continue
        size = {"word": 4, "short": 2, "byte": 1, "float": 4, "double": 8}[kind]
        raw = image[v - vram:v - vram + size]
        if kind == "double":
            for i in range(2):
                out.append(Elem(v + 4 * i, "u32", 4, struct.unpack_from("<I", raw, 4 * i)[0]))
            pos = v + 8
            continue
        val = int.from_bytes(raw, "little")
        reloc = None
        if kind == "word" and not re.fullmatch(r"-?(0x[0-9A-Fa-f]+|\d+)", op):
            reloc = parse_expr(op)
        k = {"word": "u32", "short": "u16", "byte": "u8", "float": "f32"}[kind]
        if k == "f32" and f32_literal(val) is None:
            k = "u32"
        out.append(Elem(v, k, size, val, reloc))
        pos = v + size
    if pos < end:
        gap = image[pos - vram:end - vram]
        if any(gap):
            out += [Elem(pos + i, "u8", 1, c) for i, c in enumerate(gap)]
        else:
            out.append(Elem(pos, "pad", len(gap), gap))
    return out, labels, sorted(sections)[0]


def texty(r, i):
    """Byte i of r could be text: printable ASCII, tab/newline, or the game's
    font code (a byte followed by 0xF9, and the 0xF9)."""
    c = r[i]
    return 0x20 <= c < 0x7F or c in (0x09, 0x0A, 0xF9) or (i + 1 < len(r) and r[i + 1] == 0xF9)


def plausible_text(t):
    """Text rather than numbers that happen to be printable: mostly letters,
    or the game's font code (every second byte 0xF9)."""
    if len(t) >= 4 and len(t) % 2 == 0 and all(t[i] == 0xF9 for i in range(1, len(t), 2)):
        return True
    letters = sum(1 for ch in t if chr(ch).isalpha() or ch == 0x20)
    return letters >= 0.5 * len(t) and any(chr(ch).isalpha() for ch in t)


def retype_text(elems, stops=(), min_len=3):
    """Find NUL-terminated text in runs of plain integers/strings (splat often
    writes strings as words or halfwords) and turn it into str elements, each
    with the zeros after it as padding. Other bytes keep their elements."""
    out, i = [], 0
    while i < len(elems):
        e = elems[i]
        if e.reloc or e.kind == "f32":
            out.append(e)
            i += 1
            continue
        j = i + 1
        while j < len(elems) and not elems[j].reloc and elems[j].kind != "f32" and elems[j].addr == elems[j - 1].end:
            j += 1
        run = elems[i:j]
        i = j
        base = run[0].addr
        r = b"".join(x.value if isinstance(x.value, (bytes, bytearray)) else x.value.to_bytes(x.size, "little")
                     for x in run)
        marks = []  # (start, nul end, pad end)
        k = 0
        while k < len(r):
            # the header's "save" string starts each file's .rodata, also right
            # after other data
            if (base + k) % 8 == 0 and r[k:k + 5] == b"save\0":
                z = k + 5
                while z < len(r) and r[z] == 0 and base + z not in stops:
                    z += 1
                marks.append((k, k + 5, z))
                k = z
                continue
            if r[k] and (k == 0 or r[k - 1] == 0 or base + k in stops) and texty(r, k):
                m = k
                while m < len(r) and r[m] and texty(r, m) and (m == k or base + m not in stops):
                    m += 1
                if m < len(r) and r[m] == 0 and m - k >= min_len and base + m not in stops and \
                        plausible_text(r[k:m]):
                    z = m + 1
                    while z < len(r) and r[z] == 0 and base + z not in stops:
                        z += 1
                    marks.append((k, m + 1, z))
                    k = z
                    continue
                k = max(m, k + 1)
                continue
            k += 1
        if not marks:
            out += run
            continue
        covered = set()
        for s, n, z in marks:
            covered.update(range(s, z))
        new = []
        for x in run:
            lo, hi = x.addr - base, x.end - base
            if not any(o in covered for o in range(lo, hi)):
                new.append(x)
            else:
                new += [Elem(base + o, "u8", 1, r[o]) for o in range(lo, hi) if o not in covered]
        for s, n, z in marks:
            new.append(Elem(base + s, "str", n - s, r[s:n]))
            if z > n:
                new.append(Elem(base + n, "pad", z - n, r[n:z]))
        new.sort(key=lambda x: x.addr)
        out += new
    return out


def trimmed(elems):
    """Elements without trailing padding."""
    out = list(elems)
    while out and out[-1].kind == "pad":
        out.pop()
    return out


def scalar_body(o):
    """The object's elements if it can be a scalar: one element, or (for bytes and
    halfwords, which splat also uses for padding) one element and zeros."""
    body = trimmed(o.elems)
    if body and body[0].kind in ("u8", "u16") and all(
            e.kind == body[0].kind and e.value == 0 for e in body[1:]):
        return body[:1]
    return body


def obj_align(o):
    """Alignment gcc gives the object: 8 for arrays, structs and strings
    (DATA_ALIGNMENT), else the scalar's size."""
    body = scalar_body(o)
    if len(body) == 1 and body[0].kind in ("u32", "u16", "u8", "f32") and o.end - o.addr <= 8:
        return body[0].size
    return 8


def scalar_ok(o, next_start, next_align):
    body = scalar_body(o)
    if len(body) != 1 or body[0].kind not in ("u32", "u16", "u8", "f32"):
        return False
    end = body[0].end
    return -(-end // next_align) * next_align == next_start


ALIGN = {"u32": 4, "f32": 4, "u16": 2, "u8": 1, "str": 1, "pad": 1}


def struct_end(o):
    """Where the object ends once compiled: a struct of several kinds is
    padded to its alignment (the most aligned field)."""
    body = trimmed(o.elems)
    if not body:
        return o.addr
    end = body[-1].end
    kinds = {e.kind for e in body}
    if len(kinds) > 1 and not kinds <= {"str", "pad"}:
        a = max(ALIGN[k] for k in kinds)
        end = o.addr + -(-(end - o.addr) // a) * a
    return end


def plan_objects(elems, labels, start, end, syms):
    """Split the elements into objects at labels, merging labels where no object can start."""
    objs = []
    after_save = None
    for e in elems:
        names = labels.get(e.addr)
        # The header's unused `static const char[] = "save"` starts each file's
        # .rodata: an object of its own, and so is whatever follows it
        is_save = e.kind == "str" and e.value == b"save\0" and e.addr % 8 == 0
        if names or not objs or is_save or (after_save and e.kind != "pad"):
            o = Obj(names[0] if names else f"D_{e.addr:08X}", e.addr)
            o.aliases = names[1:] if names else []
            # splat's D_ label at a piece start isn't a name the code uses
            o.save = is_save and all(n == f"D_{e.addr:08X}" for n in (names or []))
            objs.append(o)
            after_save = None
        if is_save:
            after_save = o
        objs[-1].elems.append(e)
    for v, names in labels.items():  # labels inside an element (rare): fields
        if not any(e.addr == v for e in elems):
            for o in objs:
                if o.addr < v < o.addr + sum(x.size for x in o.elems):
                    o.fields += [(n, v - o.addr) for n in names]
    for o in objs:
        o.end = o.elems[-1].end

    # Two or four bytes that end where the next object starts: a u16/u32
    # scalar (a byte scalar can't leave the gap after it)
    for i, o in enumerate(objs):
        nxt_start = objs[i + 1].addr if i + 1 < len(objs) else end
        body = o.elems
        n = sum(e.size for e in body)
        if all(e.kind in ("u8", "pad") for e in body) and len(body) > 1 and n in (2, 4) and \
                o.addr % n == 0 and o.addr + n == nxt_start:
            raw = b"".join(e.value if isinstance(e.value, (bytes, bytearray)) else bytes([e.value]) for e in body)
            o.elems = [Elem(o.addr, "u16" if n == 2 else "u32", n, int.from_bytes(raw, "little"))]

    def merge(i):
        o, prev = objs[i], objs[i - 1]
        prev.elems += o.elems
        prev.fields += [(o.name, o.addr - prev.addr)] + [(n, o.addr - prev.addr) for n in o.aliases]
        prev.fields += [(n, off + o.addr - prev.addr) for n, off in o.fields]
        prev.end = o.end
        del objs[i]

    # Merge objects that can't start where they are into the previous one,
    # and the next object into a struct whose size (rounded up to its
    # alignment) reaches it
    changed = True
    while changed:
        changed = False
        for i, o in enumerate(objs):
            nxt_start = objs[i + 1].addr if i + 1 < len(objs) else end
            nxt_align = obj_align(objs[i + 1]) if i + 1 < len(objs) else natural_align(end)
            scalar = scalar_ok(o, nxt_start, nxt_align)
            need = trimmed(o.elems)[0].size if scalar else 8
            if o.addr % need and i > 0:
                merge(i)
                changed = True
                break
            if i + 1 < len(objs) and not scalar and struct_end(o) > nxt_start:
                merge(i + 1)
                changed = True
                break
    if objs and objs[0].addr % 8 and not scalar_ok(objs[0], objs[1].addr if len(objs) > 1 else end, 4):
        print(f"warning: the range starts at 0x{start:08X}, where only a scalar can start; "
              f"start at an 8-aligned address", file=sys.stderr)
    return objs


class Writer:
    def __init__(self, b, sec, syms, objs):
        self.b, self.sec, self.syms, self.objs = b, sec, syms, objs
        self.const = "const " if sec == ".rodata" else ""
        self.externs = {}  # name -> declaration
        self.defined = {o.name: o for o in objs}
        self.fields = {}
        for o in objs:
            for n, off in o.fields:
                self.fields[n] = (o, off)
            for n in o.aliases:
                self.fields[n] = (o, 0)
        self.forward = set()
        self.seen = set()
        self.saves = 0

    # --- references ---
    def ref(self, sym, add, ctx):
        """C expression for the address sym + add. ctx: 'u32', 'ptr' or 'func'."""
        if sym in self.fields:
            o, off = self.fields[sym]
            sym, add = o.name, add + off
        if sym in self.defined:
            if sym not in self.seen:
                self.forward.add(sym)
            base = f"&{sym}"
        else:
            if sym.startswith(".L"):
                cname = "L_" + sym[2:]
                self.externs[cname] = f'extern void {cname}() __asm__("{sym}");'
                sym = cname
                base = sym
            elif self.syms.is_code(sym):
                self.externs[sym] = f"extern void {sym}();"
                base = sym
            else:
                self.externs[sym] = f"extern u8 {sym}[];"
                base = sym
        if ctx == "func" and add == 0:
            return base
        if ctx == "u32":
            return f"(u32){base}" + (f" + 0x{add:X}" if add > 0 else f" - 0x{-add:X}" if add < 0 else "")
        if add == 0:
            return base if not base.startswith("&") else base
        return f"(u8 *){base} + 0x{add:X}" if add > 0 else f"(u8 *){base} - 0x{-add:X}"

    def value(self, e, ctx="u32"):
        if e.reloc:
            return self.ref(e.reloc[0], e.reloc[1], ctx)
        if e.kind == "f32":
            return f32_literal(e.value)
        if ctx in ("ptr", "func") and e.value == 0:
            return "0"
        return f"0x{e.value:X}"

    # --- objects ---
    def emit(self, o, next_start, next_align):
        """(type declaration or None, definition line, forward-declaration form)."""
        sb = scalar_body(o)
        if scalar_ok(o, next_start, next_align) and o.addr % sb[0].size == 0:
            e = sb[0]
            if e.reloc and e.kind == "u32":
                code = self.syms.is_code(e.reloc[0]) and e.reloc[1] == 0
                t, ctx = ("Func ", "func") if code else (f"void *{'const ' if self.const else ''}", "ptr")
                if code and self.const:
                    t = "const Func "
                elif not code:
                    t = f"void *{'const ' if self.const else ''}"
            else:
                t, ctx = f"{self.const}{e.kind} ", "u32"
            return None, f"{t}{o.name} = {self.value(e, ctx)}; /* {o.addr:08X} */", f"{t}{o.name}"
        body = trimmed(o.elems)
        size = o.end - o.addr  # up to the next object, padding included
        content_end = body[-1].end if body else o.addr
        natural_end = -(-content_end // next_align) * next_align
        explicit = natural_end != next_start
        span = next_start - o.addr
        kinds = {e.kind for e in body}
        c = self.const
        comment = f"/* {o.addr:08X} */"
        if o.fields:
            comment = f"/* {o.addr:08X}; fields: " + ", ".join(f"{n} +0x{off:X}" for n, off in o.fields) + " */"

        if kinds <= {"str", "pad"} and body:
            if o.save:
                self.saves += 1
                name = "save" if self.saves == 1 else f"save_{o.addr:08X}"
                note = (" /* a header's unused static: the first thing in each file's .rodata */"
                        if self.saves == 1 else " /* the next file's .rodata starts here */")
                dim = ""
                if explicit:
                    dim = f"0x{span:X}"
                    note += f" /* padded: what follows is {natural_align_of(next_start)}-aligned */"
                return None, f"static const char {name}[{dim}] = \"save\";{note}", None
            strs = [e for e in body if e.kind == "str"]
            starts = [e.addr - o.addr for e in strs]
            stride = starts[1] - starts[0] if len(starts) > 1 else 0
            # Strings packed at a fixed stride: a 2D char array
            if len(strs) > 1 and stride > 1 and starts == [i * stride for i in range(len(strs))] and \
                    all(e.size <= stride for e in strs) and (o.end - o.addr) <= len(strs) * stride + 8:
                rows = [c_string(e.value[:-1]) for e in strs]
                decoded = [font_text(e.value[:-1]) for e in strs]
                n = len(strs)
                total = n * stride
                natural_end2 = -(-(o.addr + total) // next_align) * next_align
                if natural_end2 == next_start or (o.addr + total == next_start):
                    note = f" /* {' '.join(decoded)} */" if all(decoded) else ""
                    init = "{\n    " + ",\n    ".join(rows) + ",\n}"
                    return (None, f"{c}char {o.name}[{n}][{stride}] = {init}; {comment}{note}",
                            f"{c}char {o.name}[][{stride}]")
            data = b"".join(e.value for e in body)[:-1]  # without the final NUL
            dim = f"0x{span:X}" if explicit else ""
            decoded = [font_text(e.value[:-1]) for e in strs]
            note = f" /* {' '.join(decoded)} */" if all(decoded) else ""
            return None, f"{c}char {o.name}[{dim}] = {c_string(data)}; {comment}{note}", f"{c}char {o.name}[]"

        # Bytes that are mostly text (fixed-width names without a NUL, ...): a
        # char array written as a string
        if body and kinds <= {"u8", "str", "pad"} and "u8" in kinds:
            raw = b"".join(e.value if isinstance(e.value, (bytes, bytearray)) else bytes([e.value]) for e in body)
            nz = [i for i, ch in enumerate(raw) if ch]
            if nz and sum(1 for i in nz if texty(raw, i)) >= 0.8 * len(nz):
                n = span if explicit else len(raw)
                lit = raw[:-1] if raw.endswith(b"\0") and n > len(raw) - 1 else raw
                dim = f"0x{n:X}" if (explicit or not raw.endswith(b"\0")) else ""
                return None, f"{c}char {o.name}[{dim}] = {c_string(lit)}; {comment}", f"{c}char {o.name}[]"

        if len(kinds) == 1 and "pad" not in kinds:
            k = body[0].kind
            esz = TYPES[k]
            n = len(body)
            relocs = [e.reloc for e in body if e.reloc]
            if k == "u32" and relocs and all(e.reloc or e.value == 0 for e in body):
                allfunc = all(self.syms.is_code(e.reloc[0]) and e.reloc[1] == 0 for e in body if e.reloc)
                t, ctx = ("Func", "func") if allfunc else ("void *", "ptr")
            else:
                t, ctx = k, "u32"
            if t == "void *":
                decl_t = f"void *{'const ' if c else ''}"
            else:
                decl_t = f"{c}{t} "
            if n == 1 and not explicit and obj_align(o) == esz:
                return None, f"{decl_t}{o.name} = {self.value(body[0], ctx)}; {comment}", f"{decl_t}{o.name}"
            if explicit and span % esz == 0 and span // esz >= n:
                dim = f"0x{span // esz:X}" if span // esz != n else str(n)
            elif not explicit:
                dim = ""
            else:
                dim = None
            if dim is not None:
                vals = [self.value(e, ctx) for e in body]
                per = 4 if t in ("void *", "Func") or (k == "u32" and relocs) else 8 if esz >= 2 else 16
                lines = [", ".join(vals[i:i + per]) for i in range(0, len(vals), per)]
                init = "{\n    " + ",\n    ".join(lines) + ",\n}"
                return None, f"{decl_t}{o.name}[{dim}] = {init}; {comment}", f"{decl_t}{o.name}[]"

        # Mixed: a struct, one field per run of same-kind elements; rows of
        # the same layout: an array of structs
        arr = self.struct_array(o, body, next_start, next_align)
        if arr:
            return arr
        elems = list(o.elems) if explicit else body
        fields = self.group_fields(elems, o.addr)
        if explicit:
            used = elems[-1].end if elems else o.addr
            if used < next_start:
                fields.append((used - o.addr, "u8", next_start - used, f"u8 f_{used - o.addr:X}[0x{next_start - used:X}];", "{0}"))
        tag = f"{o.name}_s"
        tdecl = f"struct {tag} {{\n    " + "\n    ".join(f[3] for f in fields) + "\n};"
        init = "{\n    " + ",\n    ".join(f[4] for f in fields) + ",\n}"
        return tdecl, f"{c}struct {tag} {o.name} = {init}; {comment}", f"{c}struct {tag} {o.name}"

    def group_fields(self, elems, base):
        """[(offset, kind, size, declaration, initializer)]: one field per run of
        same-kind elements; a string takes the padding after it."""
        fields, run = [], []

        def flush():
            if not run:
                return
            k = run[0].kind
            off = run[0].addr - base
            size = sum(e.size for e in run)
            if k == "str":
                data = b"".join(e.value for e in run)
                body = data.rstrip(b"\0")
                fields.append((off, k, size, f"char f_{off:X}[0x{size:X}];", c_string(body)))
            elif k == "pad":
                fields.append((off, k, size, f"u8 f_{off:X}[0x{size:X}];", "{0}"))
            else:
                vals = [self.value(e, "u32") for e in run]
                if len(run) == 1:
                    fields.append((off, k, size, f"{k} f_{off:X};", vals[0]))
                else:
                    fields.append((off, k, size, f"{k} f_{off:X}[{len(run)}];", "{" + ", ".join(vals) + "}"))
            run.clear()

        for e in elems:
            if run and (e.kind == run[-1].kind and e.kind not in ("str",) or
                        e.kind == "pad" and run[-1].kind == "str"):
                run.append(e)
                continue
            flush()
            run.append(e)
        flush()
        return fields

    def struct_array(self, o, body, next_start, next_align):
        """An array of structs if the object is rows of one layout (the last
        row may be cut short: the rest is zero), else None."""
        c = self.const
        length = body[-1].end - o.addr
        align = max(ALIGN[e.kind] for e in body)
        for period in sorted({e.addr - o.addr for e in body if e.addr > o.addr}):
            if period % align or period * 2 > length:
                continue
            n = -(-length // period)
            # zero words after the last full row that the next object's
            # alignment recreates are padding, not a cut-short row
            full = length // period
            extra = [e for e in body if e.addr - o.addr >= full * period]
            use = body
            if full >= 2 and extra and all(not e.reloc and not any(
                    e.value if isinstance(e.value, (bytes, bytearray)) else [e.value]) for e in extra) and \
                    -(-(o.addr + full * period) // next_align) * next_align == next_start:
                n = full
                use = [e for e in body if e.addr - o.addr < full * period]
            rows = [[e for e in use if r * period <= e.addr - o.addr < (r + 1) * period] for r in range(n)]
            if any(not r or r[0].addr - o.addr != i * period for i, r in enumerate(rows)):
                continue
            if any(e.end - o.addr > (i + 1) * period for i, r in enumerate(rows) for e in r):
                continue  # an element crosses a row boundary
            layouts = [self.group_fields(r, o.addr + i * period) for i, r in enumerate(rows)]
            ref = [(f[0], f[1], f[2]) for f in layouts[0]]
            ok = True
            for i, lay in enumerate(layouts[1:], 1):
                sig = [(f[0], f[1], f[2]) for f in lay]
                last = i == n - 1
                if sig == ref:
                    continue
                # the last row: a prefix of the layout (its tail is zeros), or a
                # string field cut short by the end
                if last and len(sig) <= len(ref) and sig[:-1] == ref[:len(sig) - 1] and \
                        sig[-1][:2] == ref[len(sig) - 1][:2] and sig[-1][2] <= ref[len(sig) - 1][2]:
                    continue
                ok = False
                break
            if not ok:
                continue
            # the rows must reach where the next object starts (padding recreated)
            row_end = o.addr + n * period
            if row_end > next_start or -(-row_end // next_align) * next_align != next_start:
                continue
            tag = f"{o.name}_s"
            tdecl = f"struct {tag} {{\n    " + "\n    ".join(f[3] for f in layouts[0]) + "\n};"
            rows_txt = ["{ " + ", ".join(f[4] for f in lay) + " }" for lay in layouts]
            init = "{\n    " + ",\n    ".join(rows_txt) + ",\n}"
            comment = f"/* {o.addr:08X}"
            if o.fields:
                comment += "; fields: " + ", ".join(f"{nm} +0x{off:X}" for nm, off in o.fields)
            comment += " */"
            return tdecl, f"{c}struct {tag} {o.name}[{n}] = {init}; {comment}", f"{c}struct {tag} {o.name}[]"
        return None


def floaty(w):
    """A word that reads as a plausible float: 0, or a normal number between
    2^-16 and 2^20 with the low 12 mantissa bits clear."""
    if w in (0, 0x80000000):
        return True
    exp = (w >> 23) & 0xFF
    return 0x6F <= exp <= 0x93 and w & 0xFFF == 0


def floatify(o):
    """Plain words that all read as floats (splat types floats by value, and
    misses some) become f32."""
    body = trimmed(o.elems)
    words = [e for e in body if e.kind in ("u32", "f32")]
    if not body or len(words) != len(body) or any(e.reloc for e in body):
        return
    nonzero = [e for e in words if e.value not in (0, 0x80000000)]
    if len(nonzero) >= 2 and all(floaty(e.value) for e in words) and \
            all(f32_literal(e.value) for e in words):
        for e in words:
            e.kind = "f32"


def generate(b, start, end, image=None, source=None):
    image = image if image is not None else rom(b)
    syms = Symbols(b)
    elems, labels, sec = elements(b, start, end, image, syms)
    elems = retype_text(elems, set(labels))
    objs = plan_objects(elems, labels, start, end, syms)
    for o in objs:
        floatify(o)
    w = Writer(b, sec, syms, objs)
    types, defs, fwd_forms = [], [], {}
    for i, o in enumerate(objs):
        nxt_start = objs[i + 1].addr if i + 1 < len(objs) else end
        nxt_align = obj_align(objs[i + 1]) if i + 1 < len(objs) else natural_align(end)
        t, d, fwd = w.emit(o, nxt_start, nxt_align)
        w.seen.add(o.name)
        fwd_forms[o.name] = fwd
        if t:
            types.append(t)
        defs.append(d)
    kind = sec[1:]
    lines = [
        f"/* {BINARIES[b]['name'].upper() if b != 'main' else 'main executable'} {sec} "
        f"0x{start:08X}-0x{end:08X}: data-only C (plan step 6b, docs/shifting.md).",
        f" * Drafted by tools/gen_data_c.py from {source or 'the asm'}; check with",
        f" * tools/check_data.py. Don't include the project headers here: declare what",
        f" * the data refers to locally (extern u8 x[] is fine for an address).",
        " */",
        f"/* {kind}: 0x{start:08X}-0x{end:08X} */",
        "",
        "typedef unsigned char u8;",
        "typedef unsigned short u16;",
        "typedef unsigned int u32;",
        "typedef float f32;",
    ]
    if any("Func " in d for d in defs):
        lines.append("typedef void (*Func)();")
    lines.append("")
    if w.externs:
        lines += [w.externs[n] for n in sorted(w.externs)]
        lines.append("")
    if types:
        lines += types
        lines.append("")
    if w.forward:
        lines += [f"extern {fwd_forms[n]};" for n in sorted(w.forward)]
        lines.append("")
    lines += defs
    return "\n".join(lines) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("binary", choices=sorted(BINARIES))
    ap.add_argument("start")
    ap.add_argument("end")
    ap.add_argument("-o", "--out")
    a = ap.parse_args()
    start, end = int(a.start, 16), int(a.end, 16)
    b, _ = binary_for_data(start)
    if b != a.binary:
        raise SystemExit(f"0x{start:08X} is in {b}'s data, not {a.binary}'s")
    pieces = asm_pieces(b, start, end)
    src = ", ".join(os.path.relpath(p.path) for p in pieces)
    text = generate(b, start, end, source=src)
    if a.out:
        os.makedirs(os.path.dirname(os.path.abspath(a.out)), exist_ok=True)
        open(a.out, "w").write(text)
        print(f"wrote {a.out}")
    else:
        sys.stdout.write(text)


if __name__ == "__main__":
    main()
