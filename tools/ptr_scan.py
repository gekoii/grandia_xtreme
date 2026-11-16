#!/usr/bin/env python3
"""Accesses through pointers that the relocations don't show: for each
address formed by a lui/%lo pair (a symbol S), the bytes the code then reads
or writes through it at constant offsets. tools/pin_link.py uses them to keep
units together that are reached from one another's symbol.

Usage: ptr_scan.py [binary ...]   (lists the accesses that leave S's object)

The relocations give `S+off` only when the offset is in the %lo. These
aren't in them:
  * a pointer kept in a register:  la a0, movVideoDec; ...; lw v0, 0xB0(a0)
    (the code reaches movVideoDecViBuf, which splat made a separate object);
  * a pointer passed to a function that uses it at constant offsets (the
    offsets each function reaches through each argument register, also
    through the functions it passes the pointer to);
  * a pointer and a constant size passed to memset, MemSet, memcpy,
    memmove or VuCopy128.
Each lui is followed along every path from it (branches followed, calls end
the caller-saved registers, a loop is followed once), like tools/abi_scan.py
does for the disc's programs. Index registers (`addu` of a variable) end the
tracking: an indexed access has no constant extent.
"""
import bisect
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elf32  # noqa: E402
from abi_scan import CALLER_SAVED, LOADS_GPR, MEM_OPS, sext16, successors, writes  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
ARGS = (4, 5, 6, 7)
# bytes each load/store touches (lq/sq and the COP2 quadword ones: 16)
WIDTH = {0x20: 1, 0x24: 1, 0x28: 1, 0x21: 2, 0x25: 2, 0x29: 2, 0x22: 4, 0x23: 4, 0x26: 4, 0x27: 4,
         0x2A: 4, 0x2B: 4, 0x2E: 4, 0x31: 4, 0x39: 4, 0x1A: 8, 0x1B: 8, 0x2C: 8, 0x2D: 8, 0x37: 8,
         0x3F: 8, 0x35: 8, 0x3D: 8, 0x1E: 16, 0x1F: 16, 0x36: 16, 0x3E: 16}
# functions whose extent is a size argument: [(pointer register, size)], the
# size a constant or "a2" (the constant the caller puts in $a2)
SIZED = {"memset": [(4, "a2")], "MemSet": [(4, "a2")], "memcpy": [(4, "a2"), (5, "a2")],
         "memmove": [(4, "a2"), (5, "a2")], "VuCopy128": [(4, 0x80), (5, 0x80)]}
# functions whose extent depends on the data (strings, sizes in registers)
OPAQUE = {"strcpy", "strncpy", "strcat", "strcmp", "strncmp", "strlen", "memcmp", "sprintf",
          "printf", "scePrintf", "FlushCache", "SyncDCache", "InvalidDCache", "iSyncDCache",
          "iInvalidDCache", "sceSifLoadModule"}


class Code:
    """The code of a linked binary, or with words, a piece of code from
    elsewhere (a disc program: [(address, words)], no functions or names)."""

    def __init__(self, binary, elf_path=None, words=None):
        self.binary = binary
        if words is not None:
            self.secs, self.funcs, self.names = words, [], {}
            return
        e = elf32.Elf(os.path.join(ROOT, elf_path))
        self.secs = []
        for s in e.alloc_sections():
            if s["flags"] & 4:  # SHF_EXECINSTR
                d = e.data(s)
                self.secs.append((s["addr"], list(struct.unpack_from(f"<{len(d) // 4}I", d))))
        self.funcs = sorted(int(line.split()[0], 16) for line in
                            open(os.path.join(ROOT, f"config/functions.{binary}.txt")) if line.strip())
        self.names = {}
        for s in e.symbols():
            if s["name"] and s["shndx"] != elf32.SHN_UNDEF:
                self.names.setdefault(s["value"], set()).add(s["name"])


def const_at(words, j, reg):
    """The constant in reg at the call at j (li/ori/lui+ori/addiu in the 24
    instructions before it, or its delay slot), or None."""
    val = None
    for k in list(range(max(0, j - 24), j)) + [j + 1]:
        if k >= len(words):
            break
        x = words[k]
        op, rs, rt = x >> 26, (x >> 21) & 31, (x >> 16) & 31
        if op in (0x09, 0x19) and rs == 0 and rt == reg:
            val = sext16(x & 0xFFFF)
        elif op == 0x0D and rs == 0 and rt == reg:
            val = x & 0xFFFF
        elif op == 0x0F and rt == reg:
            val = (x & 0xFFFF) << 16
        elif op == 0x0D and rs == reg and rt == reg and val is not None:
            val |= x & 0xFFFF
        elif op in (0x09, 0x19) and rs == reg and rt == reg and val is not None:
            val += sext16(x & 0xFFFF)
        elif writes(x) == reg:
            val = None
    return val


def walk(words, start, n, state, visit, lo=0, hi=None, budget=4000):
    """Follow the paths from instruction start with register state; visit(j,
    state) returns the state after instruction j (a dict, changed in place or
    new). Calls drop the caller-saved registers; each instruction is visited
    once."""
    hi = n if hi is None else hi
    seen = set()
    work = [(start, state)]
    while work and budget > 0:
        j, st = work.pop()
        while lo <= j < hi and st and budget > 0:
            if j in seen:
                break
            seen.add(j)
            budget -= 1
            nxt = successors(words, j, n)
            st = visit(j, st)
            if nxt is None:
                j += 1
                continue
            if j + 1 < n:
                st = visit(j + 1, st)
            targets, call = nxt
            if call:
                st = {r: v for r, v in st.items() if r not in CALLER_SAVED}
            if not targets:
                break
            for k in targets[1:]:
                work.append((k, dict(st)))
            j = targets[0]


def track(x, st):
    """Register moves and constant offsets of a pointer state {reg: (base, off)};
    returns True if x was handled."""
    op, rs, rt, rd = x >> 26, (x >> 21) & 31, (x >> 16) & 31, (x >> 11) & 31
    if op in (0x09, 0x19) and rs in st:  # addiu/daddiu
        st[rt] = (st[rs][0], st[rs][1] + sext16(x & 0xFFFF))
        return True
    if op == 0 and (x & 63) in (0x21, 0x2D, 0x25) and 0 in (rs, rt) and (rs or rt) in st:  # move
        st[rd] = st[rs or rt]
        return True
    return False


def arg_extents(codes):
    """{(function, arg register): (lo, hi)}: the byte offsets each function
    reaches at constant offsets through each argument register, including
    through the functions it passes it to."""
    ext, passes = {}, []
    by_name = {}
    for c in codes:
        for a, ns in c.names.items():
            for nm in ns:
                by_name.setdefault(a, set()).add(nm)
    skip = {a for a, ns in by_name.items() if ns & (OPAQUE | set(SIZED))}
    for c in codes:
        for base, words in c.secs:
            n = len(words)
            fs = [f for f in c.funcs if base <= f < base + 4 * n]
            for k, f in enumerate(fs):
                lo_i = (f - base) // 4
                hi_i = ((fs[k + 1] if k + 1 < len(fs) else base + 4 * n) - base) // 4

                def visit(j, st, f=f, words=words, record=True):
                    x = words[j]
                    op, rs, rt = x >> 26, (x >> 21) & 31, (x >> 16) & 31
                    st = dict(st)
                    if op == 3 and record and j + 1 < len(words):  # the arguments after the delay slot
                        args = visit(j + 1, st, record=False)
                        t = (x & 0x03FFFFFF) << 2
                        for r in ARGS:
                            if r in args:
                                passes.append((f, args[r][0], t, r, args[r][1]))
                        return st
                    if op in MEM_OPS and rs in st and op in WIDTH:
                        r0, o = st[rs]
                        o += sext16(x & 0xFFFF)
                        if record:
                            lo, hi = ext.get((f, r0), (0, 0))
                            ext[(f, r0)] = (min(lo, o), max(hi, o + WIDTH[op]))
                        if op in LOADS_GPR:
                            st.pop(rt, None)
                        return st
                    if track(x, st):
                        return st
                    w = writes(x)
                    if w is not None:
                        st.pop(w, None)
                    return st
                walk(words, lo_i, n, {r: (r, 0) for r in ARGS}, visit, lo_i, hi_i)
    for _ in range(8):
        changed = False
        for f, r, t, tr, off in passes:
            if t in skip or (t, tr) not in ext:
                continue
            lo, hi = ext[(t, tr)]
            cur = ext.get((f, r), (0, 0))
            new = (min(cur[0], off + lo), max(cur[1], off + hi))
            if new != cur:
                ext[(f, r)] = new
                changed = True
        if not changed:
            break
    for a in skip:
        for r in ARGS:
            ext.pop((a, r), None)
    return ext, by_name


def scan(elfs, users=None, extra=()):
    """elfs: {binary: linked ELF path}. Returns [(S, lo, hi, binary, site)]:
    code of the binaries in users (default all), and the extra Codes, at
    site reaches the bytes [lo, hi) through a pointer to S."""
    codes = [Code(b, p) for b, p in elfs.items()]
    ext, by_name = arg_extents(codes)
    codes += list(extra)
    sized = {a: SIZED[nm] for a, ns in by_name.items() for nm in ns if nm in SIZED}
    out = []

    def at_call(j, t, st, words, base):
        """The pointers in the argument registers at the call at j to t."""
        for r, size in sized.get(t, ()):
            p = st.get(r)
            if p is None or p == "hi":
                continue
            if size == "a2":
                size = const_at(words, j, 6)
            if size and size > 0:
                a = p[0] + p[1]
                out.append((p[0], a, a + size, c.binary, base + 4 * j))
        for r in ARGS:
            p = st.get(r)
            if p is None or p == "hi" or (t, r) not in ext:
                continue
            lo, hi = ext[(t, r)]
            if lo < hi:
                a = p[0] + p[1]
                out.append((p[0], a + lo, a + hi, c.binary, base + 4 * j))
    for c in codes:
        if users is not None and c.binary not in users and c not in extra:
            continue
        for base, words in c.secs:
            n = len(words)
            for i, w in enumerate(words):
                if w >> 26 != 0x0F or (w >> 21) & 31:
                    continue
                hi = (w & 0xFFFF) << 16

                def visit(j, st, words=words, base=base, hi=hi, record=True):
                    x = words[j]
                    op, rs, rt, rd = x >> 26, (x >> 21) & 31, (x >> 16) & 31, (x >> 11) & 31
                    st = dict(st)
                    imm = sext16(x & 0xFFFF)
                    v = st.get(rs)
                    if op == 3 and record and j + 1 < len(words):  # the arguments after the delay slot
                        at_call(j, (x & 0x03FFFFFF) << 2, visit(j + 1, st, record=False), words, base)
                        return st
                    if op in MEM_OPS and v is not None:
                        if v != "hi" and op in WIDTH and record:  # through a pointer
                            a = v[0] + v[1] + imm
                            out.append((v[0], a, a + WIDTH[op], c.binary, base + 4 * j))
                        if op in LOADS_GPR:
                            st.pop(rt, None)
                        return st
                    if op in (0x09, 0x19) and v is not None:
                        st[rt] = (hi + imm, 0) if v == "hi" else (v[0], v[1] + imm)
                        return st
                    if op == 0 and (x & 63) in (0x21, 0x2D, 0x25) and 0 in (rs, rt) and (rs or rt) in st:
                        st[rd] = st[rs or rt]
                        return st
                    w2 = writes(x)
                    if w2 is not None:
                        st.pop(w2, None)
                    return st
                start = (w >> 16) & 31
                prev = successors(words, i - 1, n) if i > 0 else None
                if prev is not None:  # the lui is a delay slot: follow from the targets
                    targets, call = prev
                    for k in targets:
                        if not (call and start in CALLER_SAVED):
                            walk(words, k, n, {start: "hi"}, visit, budget=600)
                else:
                    walk(words, i + 1, n, {start: "hi"}, visit, budget=600)
    return out


def main():
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import ld_script
    bins = sys.argv[1:] or ["main", "field", "battle", "viewer"]
    elfs = {b: f"build/{ld_script.elf_name(b)}.elf" for b in ("main", "field", "battle", "viewer")}
    syms = {}
    for b, p in elfs.items():
        for s in elf32.Elf(os.path.join(ROOT, p)).symbols():
            if s["name"] and s["type"] == 1 and s["size"]:
                syms[s["value"]] = (s["name"], s["size"])
    starts = sorted(syms)
    seen = set()
    for S, lo, hi, b, site in scan(elfs, bins):
        if S not in syms:
            continue
        name, size = syms[S]
        if lo < S or hi > S + size:
            key = (S, lo, hi)
            if key in seen:
                continue
            seen.add(key)
            i = bisect.bisect_right(starts, hi - 1) - 1
            print(f"{b}:{site:08X} {name} ({S:08X}, {size:#x} bytes) reaches {lo:08X}-{hi:08X}"
                  f" ({syms[starts[i]][0]})")


if __name__ == "__main__":
    main()
