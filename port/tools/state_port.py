#!/usr/bin/env python3
"""Carry a save state over to another build of grandia.exe (port/docs/savestates.md).

    python port/tools/state_port.py OLD.map NEW.exe NEW.map IN.grxstate OUT.grxstate

A save state only loads in the exe that made it: its stack holds return
addresses into the exe's code, and the game's memory holds pointers to the
exe's functions and constant data. Between two builds whose data and bss
are where they were (the game's globals; checked), this rewrites each such
pointer for the new exe and stamps the new exe's build id (state.c's
exe_info: FNV-1a over .text, with SizeOfImage). Code maps by the function
it falls in, in each link map (by its object's input section, its name and
which of that name it is there): the new function's start plus the offset
into it. Constant data maps by the input section it falls in (an object's
.rdata, ...: its local data has no symbols), as does code before an input
section's first symbol.

A pointer is a 32-bit value, at a 4-byte boundary (the image is below
4 GB and the game keeps host pointers in 32-bit slots too), that is a
function's start or falls in the old exe's constant data; on the stack,
anywhere in its code (return addresses). A number that only looks like
one is rewritten as well: it says how many it rewrote, where, and which
fell inside a function (or data section) whose size changed (a changed
function: only its start maps for sure).
Load the result and check it; keep the old state.
"""

import bisect
import re
import struct
import sys

import numpy as np

MAGIC = b"GRXSTATE"
VERSION = 1
IMAGE_BASE = 0x04000000
MOVABLE = (".text", ".rdata")

_LINE = re.compile(r"^([0-9a-f]{8}) ([0-9a-f]{8}) +(\d+)( +)(\S.*)$")


def read_map(path):
    """Output sections {name: (rva, size)}, input sections [(rva, size, key)],
    the code's symbols (their rvas) and each input section's symbols
    {key: [(rva, name, n)]} (n: which of that name in the section)"""
    out, ins, seen, cur, code = {}, [], {}, None, set()
    syms, nsym, key = {}, {}, None
    with open(path, encoding="utf-8", errors="replace") as f:
        next(f)
        for line in f:
            m = _LINE.match(line.rstrip("\n"))
            if not m:
                continue
            rva, size, rest = int(m.group(1), 16), int(m.group(2), 16), m.group(5)
            if len(m.group(4)) == 1:  # an output section (symbols are further in)
                cur = rest.split()[0]
                out[cur] = (rva, size)
                key = None
                continue
            p = rest.rfind(":(")
            if p < 0 and cur == ".text":
                code.add(rva)
            if p < 0 and key is not None:
                n = nsym.get((key, rest), 0)
                nsym[key, rest] = n + 1
                syms.setdefault(key, []).append((rva, rest, n))
            if p >= 0:
                key = None
            if p < 0 or cur not in MOVABLE:
                continue
            obj, sec = rest[:p].replace("\\", "/"), rest[p + 2:-1]
            # the object's path from its build's obj/ (the output folders differ)
            q = obj.find("/obj/")
            if q >= 0:
                obj = obj[q:]
            k = (obj, sec)
            n = seen.get(k, 0)
            seen[k] = n + 1
            key = (obj, sec, n)
            ins.append((rva, size, key))
    ins.sort()
    return out, ins, code, syms


def sym_sizes(ins, syms):
    """{key: [(rva, size, name, n)]}: each symbol's size, to the next symbol
    at a higher address or the input section's end"""
    end = {key: rva + size for rva, size, key in ins}
    res = {}
    for key, lst in syms.items():
        starts = sorted({e[0] for e in lst})
        res[key] = []
        for rva, name, n in sorted(lst, key=lambda e: e[0]):
            j = bisect.bisect_right(starts, rva)
            nxt = starts[j] if j < len(starts) else end[key]
            # (max: an end marker can sit at the section's end)
            res[key].append((rva, max(nxt - rva, 0), name, n))
    return res


def build_id(exe):
    with open(exe, "rb") as f:
        d = f.read()
    pe = struct.unpack_from("<I", d, 0x3C)[0]
    nsec = struct.unpack_from("<H", d, pe + 6)[0]
    opt = struct.unpack_from("<H", d, pe + 20)[0]
    size_of_image = struct.unpack_from("<I", d, pe + 24 + 56)[0]
    h = 0xCBF29CE484222325
    sec = pe + 24 + opt
    for i in range(nsec):
        name, vsize, va, rawsize, rawptr = struct.unpack_from("<8sIIII", d, sec + 40 * i)
        if name == b".text\0\0\0":
            data = d[rawptr:rawptr + vsize].ljust(vsize, b"\0")
            for b in data:
                h = ((h ^ b) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return h ^ (size_of_image << 1)


class Mapper:
    def __init__(self, old_map, new_map):
        self.old_out, old_in, self.code, old_syms = read_map(old_map)
        self.new_out, new_in, _, new_syms = read_map(new_map)
        self.text = (IMAGE_BASE + self.old_out[".text"][0],
                     IMAGE_BASE + self.old_out[".text"][0] + self.old_out[".text"][1])
        for s in (".data", ".bss", ".buildid"):
            if s in self.old_out and self.old_out.get(s) != self.new_out.get(s):
                sys.exit(f"{s} differs between the builds ({self.old_out[s]} vs {self.new_out.get(s)}): "
                         "the game's globals moved, a state can't be carried over")
        new_at = {k: (rva, size) for rva, size, k in new_in}
        self.starts = [s[0] for s in old_in]
        self.secs = old_in
        self.new_at = new_at
        # the code's symbols by input section, and the new map's by (input
        # section, name, which of that name)
        self.syms = {k: v for k, v in sym_sizes(old_in, old_syms).items() if k[1].startswith(".text")}
        self.sym_starts = {k: [e[0] for e in v] for k, v in self.syms.items()}
        self.new_sym_at = {(k, name, n): (rva, size)
                           for k, v in sym_sizes(new_in, new_syms).items() for rva, size, name, n in v}
        lo = min(self.old_out[s][0] for s in MOVABLE)
        hi = max(self.old_out[s][0] + self.old_out[s][1] for s in MOVABLE)
        self.lo, self.hi = IMAGE_BASE + lo, IMAGE_BASE + hi

    def map(self, va):
        """(new va, note): note None, 'resized' (offset into a function or section of
        another size) or 'lost'"""
        rva = va - IMAGE_BASE
        i = bisect.bisect_right(self.starts, rva) - 1
        # back over the empty sections to the one that holds rva; none if
        # the last non-empty one ends before it (padding between sections)
        while i >= 0:
            start, size = self.secs[i][0], self.secs[i][1]
            if size and rva < start + size:
                break
            if size:
                i = -1
                break
            i -= 1
        if i < 0:
            return va, "lost"
        start, size, key = self.secs[i]
        if key not in self.new_at:
            return va, "lost"
        # code: by the function it falls in (the first name at its address)
        starts = self.sym_starts.get(key, [])
        j = bisect.bisect_right(starts, rva) - 1
        if j >= 0:
            start, size, name, n = self.syms[key][bisect.bisect_left(starts, starts[j])]
            if (key, name, n) not in self.new_sym_at:
                return va, "lost"
            nstart, nsize = self.new_sym_at[key, name, n]
        else:
            nstart, nsize = self.new_at[key]
        off = rva - start
        note = None if nsize == size or off == 0 else "resized"
        if off >= nsize and nsize != 0:
            note = "lost"
        return IMAGE_BASE + nstart + off, note

    def section_of(self, va):
        rva = va - IMAGE_BASE
        i = bisect.bisect_right(self.starts, rva) - 1
        return self.secs[i][2] if i >= 0 else None


def relocate(buf, base_off, mp, where, report, returns=False):
    """Rewrite the 32-bit words of buf (a bytearray) at 4-byte boundaries from
    base_off: pointers to constant data, and to code where a function starts
    (or anywhere in it, `returns`: the stack's return addresses)"""
    n = (len(buf) - base_off) // 4
    if n <= 0:
        return 0
    a = np.frombuffer(memoryview(buf)[base_off:base_off + 4 * n], dtype="<u4")
    idx = np.nonzero((a >= mp.lo) & (a < mp.hi))[0]
    done = 0
    for i in idx:
        v = int(a[i])
        if not returns and mp.text[0] <= v < mp.text[1] and v - IMAGE_BASE not in mp.code:
            continue  # inside a function: a number that looks like a code address
        nv, note = mp.map(v)
        if note:
            report.append((where, base_off + 4 * int(i), v, nv, note, mp.section_of(v)))
        if nv != v and note != "lost":
            struct.pack_into("<I", buf, base_off + 4 * int(i), nv)
            done += 1
    return done


def decompress(d, pos, size):
    out = bytearray(size)
    i = 0
    while i < size:
        z, l = struct.unpack_from("<II", d, pos)
        pos += 8
        i += z
        out[i:i + l] = d[pos:pos + l]
        pos += l
        i += l
    return out, pos


def compress(buf):
    """state.c's runs: (zero bytes, literal bytes, literals), zero runs of 16 bytes or more"""
    parts, i, n = [], 0, len(buf)
    for m in re.finditer(rb"\0{16,}", bytes(buf)):
        a, b = m.start(), m.end()
        parts.append(struct.pack("<II", 0, a - i) + bytes(buf[i:a]))
        parts.append(struct.pack("<II", b - a, 0))
        i = b
    if i < n:
        parts.append(struct.pack("<II", 0, n - i) + bytes(buf[i:]))
    return b"".join(parts)


def main():
    if len(sys.argv) != 6:
        sys.exit(__doc__)
    old_map, new_exe, new_map, src, dst = sys.argv[1:]
    mp = Mapper(old_map, new_map)
    with open(src, "rb") as f:
        d = f.read()
    if d[:8] != MAGIC or struct.unpack_from("<I", d, 8)[0] != VERSION:
        sys.exit(f"{src}: not a save state of version {VERSION}")
    if struct.unpack_from("<I", d, 12)[0] != 8:
        sys.exit(f"{src}: saved by the 32-bit build (this does the 64-bit one)")
    out = bytearray(d[:16])
    out += struct.pack("<Q", build_id(new_exe))
    boundary, game_top, wait_site = struct.unpack_from("<QQQ", d, 24)
    nwait, note = mp.map(wait_site)
    if note:
        sys.exit(f"the game's wait site 0x{wait_site:08X} doesn't map ({note})")
    out += struct.pack("<QQQ", boundary, game_top, nwait)
    pos = 48
    report, total = [], 0
    nreg = struct.unpack_from("<I", d, pos)[0]
    pos += 4
    out += struct.pack("<I", nreg)
    for r in range(nreg):
        addr, size = struct.unpack_from("<QQ", d, pos)
        pos += 16
        buf, pos = decompress(d, pos, size)
        k = relocate(buf, 0, mp, f"region 0x{addr:08X}", report)
        total += k
        print(f"region 0x{addr:08X} +0x{size:X}: {k} pointers")
        out += struct.pack("<QQ", addr, size) + compress(buf)
    nparts = struct.unpack_from("<I", d, pos)[0]
    pos += 4
    out += struct.pack("<I", nparts)
    for p in range(nparts):
        e = d.index(b"\0", pos)
        name = d[pos:e].decode()
        pos = e + 1
        size = struct.unpack_from("<I", d, pos)[0]
        pos += 4
        buf = bytearray(d[pos:pos + size])
        pos += size
        k = relocate(buf, 0, mp, f"part {name}", report)
        total += k
        if k:
            print(f"part {name}: {k} pointers")
        out += name.encode() + b"\0" + struct.pack("<I", size) + buf
    ssize = struct.unpack_from("<Q", d, pos)[0]
    pos += 8
    stack = bytearray(d[pos:pos + ssize])
    pos += ssize
    # the stack runs from game_top - ssize; keep 4-byte boundaries of the address
    k = relocate(stack, (-(game_top - ssize)) % 4, mp, "stack", report, returns=True)
    total += k
    print(f"stack: {k} pointers")
    out += struct.pack("<Q", ssize) + stack
    if pos != len(d):
        sys.exit(f"{src}: {len(d) - pos} bytes after the stack")
    for where, off, v, nv, note, sec in report[:40]:
        print(f"  {note}: {where} +0x{off:X}: 0x{v:08X} -> 0x{nv:08X} in {sec}")
    if len(report) > 40:
        print(f"  ... {len(report) - 40} more")
    print(f"{total} pointers rewritten, {sum(1 for r in report if r[4] == 'lost')} left as they were, "
          f"{sum(1 for r in report if r[4] == 'resized')} into resized sections")
    with open(dst, "wb") as f:
        f.write(out)


if __name__ == "__main__":
    main()
