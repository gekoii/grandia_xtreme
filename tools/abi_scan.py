#!/usr/bin/env python3
"""Scan the disc's external programs for the main-executable and BATTLE
addresses they use, and write the ABI lists config/abi_main.txt and
config/abi_battle.txt.

Usage: abi_scan.py [--iso DIR] [--write] [-v]

The character programs (SYS/GBX.PAC) and the magic and skill programs
(BTL_DATA/*.DAT) are MIPS code linked at fixed addresses (0x708000..
0x747FFF and 0x748000). They call main and BATTLE functions and use main and
BATTLE globals by absolute address, so those addresses are an ABI that a
relinked main or BATTLE must keep (docs/shifting.md, "The ABI").

What counts as a program and how it is split into code and data:
  BTL_DATA/*.DAT  a pack {count, (offset, size)[count]}; entry 9 is the
                  program BATTLE copies to 0x748000 (b:6B1290). Other entries
                  are models, textures and motions.
  SYS/GBX.PAC     a table of 192 offsets; each non-empty entry is one
                  character program (linked at its GBX slot).
  other SYS/*.PAC, SYS/FACE.DAT  data only (checked: no code).
Inside a program, code runs from the start to the last `jr ra` (+ delay
slot) before the first "save" string, which, as in the game's own
binaries, starts .rodata; the rest is data.

What is collected, per target address in main or BATTLE:
  jal / j    calls and tail calls (the target must be a function start in
             config/functions.<binary>.txt; anything else is reported).
  hi/lo      a `lui` followed, on the same path, by an addiu or a load or
             store based on that register (also through addu/daddu, as in
             `lui; addu; lw %lo(table)(reg)`): the full address.
  hi only    a `lui` with an upper half in main/BATTLE whose %lo wasn't
             found; listed separately (counts only).
  pointer    a word in a program's data that equals a main/BATTLE function
             start or a named symbol's address.

Names come from the symbol files (config/symbol_addrs*.txt) and, for
addresses without an entry, from the built ELFs (build/SLUS_204.17.elf,
build/battle.elf): `name` or `name+0xOFF` for an address inside an object.
The lists hold only addresses and our own names, no disc data.
"""
import argparse
import collections
import glob
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elf32  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

# Memory each binary occupies when loaded (image + bss)
RANGES = {
    "main": (0x00400000, 0x005A6B20),
    "field": (0x005B0000, 0x0067F650),
    "battle": (0x00680000, 0x00700000),  # incl. btlDbgStage 0x6FFFFC
}
ELFS = {"main": "build/SLUS_204.17.elf", "field": "build/field.elf", "battle": "build/battle.elf"}
SYMFILES = {"main": ["config/symbol_addrs.txt"],
            "field": ["config/symbol_addrs.field.txt", "config/symbol_addrs.txt"],
            "battle": ["config/symbol_addrs.battle.txt", "config/symbol_addrs.txt"]}
OUTPUTS = {"main": "config/abi_main.txt", "battle": "config/abi_battle.txt"}


def binary_of(a):
    for b, (lo, hi) in RANGES.items():
        if lo <= a < hi:
            return b
    return None


# ------------------------------------------------------------------ names
def functions(b):
    out = set()
    for line in open(os.path.join(ROOT, f"config/functions.{b}.txt")):
        m = re.match(r"\s*(?:0x)?([0-9A-Fa-f]{6,8})\b", line)
        if m:
            out.add(int(m.group(1), 16))
    return out


SYM_RE = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);")


class Names:
    """Address -> name for one binary."""

    def __init__(self, b):
        self.b = b
        lo, hi = RANGES[b]
        self.exact = {}
        for f in SYMFILES[b]:
            for line in open(os.path.join(ROOT, f)):
                m = SYM_RE.match(line)
                if m:
                    a = int(m.group(2), 16)
                    if lo <= a < hi:
                        self.exact.setdefault(a, m.group(1))
        self.objs = []  # (start, end, name) from the ELF: sized objects and functions
        self.labels = {}  # every defined label in range
        e = elf32.Elf(os.path.join(ROOT, ELFS[b]))
        for s in e.symbols():
            if not s["name"] or s["shndx"] in (elf32.SHN_UNDEF,) or s["name"].startswith((".", "__shift")):
                continue
            v = s["value"]
            if not lo <= v < hi:
                continue
            self.labels.setdefault(v, s["name"])
            if s["size"] and s["type"] in (1, 2):
                self.objs.append((v, v + s["size"], s["name"]))
        self.objs.sort()
        self.sorted_labels = sorted(self.labels)

    def name(self, a):
        if a in self.exact:
            return self.exact[a]
        # the innermost sized object holding a, else the closest label before it
        best = None
        for s, e, n in self.objs:
            if s > a:
                break
            if s <= a < e and (best is None or s >= best[0]):
                best = (s, n)
        if best is None:
            import bisect
            i = bisect.bisect_right(self.sorted_labels, a) - 1
            if i >= 0:
                s = self.sorted_labels[i]
                best = (s, self.labels[s])
        if best is None:
            return f"D_{a:08X}"
        s, n = best
        n = self.exact.get(s, n)
        return n if s == a else f"{n}+{a - s:#x}"


# ------------------------------------------------------------------ programs
def pack_entries(d):
    n = struct.unpack_from("<I", d, 0)[0] if len(d) >= 4 else 0
    if not 0 < n < 4096 or 4 + 8 * n > len(d):
        return None
    ents = [struct.unpack_from("<II", d, 4 + 8 * i) for i in range(n)]
    if any(o and o + s > len(d) for o, s in ents):
        return None
    return ents


def programs(iso):
    """(file, entry, bytes) for every external program on the disc."""
    out = []
    for f in sorted(glob.glob(os.path.join(iso, "BTL_DATA", "*.DAT"))):
        d = open(f, "rb").read()
        ents = pack_entries(d)
        if ents and len(ents) > 9 and ents[9][0] and ents[9][1]:
            o, s = ents[9]
            out.append((os.path.relpath(f, iso), 9, d[o:o + s]))
    gbx = os.path.join(iso, "SYS", "GBX.PAC")
    d = open(gbx, "rb").read()
    first = struct.unpack_from("<I", d, 0)[0]
    offs = list(struct.unpack_from(f"<{first // 4}I", d, 0)) + [len(d)]
    for i in range(first // 4):
        if offs[i + 1] > offs[i]:
            out.append(("SYS/GBX.PAC", i, d[offs[i]:offs[i + 1]]))
    return out


def data_files(iso):
    """Files that hold no code but might hold absolute pointers."""
    out = [f for f in sorted(glob.glob(os.path.join(iso, "SYS", "*"))) if not f.endswith("GBX.PAC")]
    return out


def code_end(p):
    save = p.find(b"save\0")
    limit = save if save > 0 else len(p)
    end = 0
    for a in range(0, limit - 3, 4):
        if struct.unpack_from("<I", p, a)[0] == 0x03E00008:
            end = a + 8
    return end


# ------------------------------------------------------------------ decoding
MEM_OPS = {0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,
           0x2D, 0x2E, 0x31, 0x36, 0x37, 0x39, 0x3E, 0x3F}
LOADS_GPR = {0x1E, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x37}
CALLER_SAVED = set(range(1, 16)) | {24, 25, 31}


def sext16(x):
    return x - 0x10000 if x & 0x8000 else x


def writes(w):
    """The GPR an instruction writes (None if none or unknown)."""
    op = w >> 26
    rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    if op == 0:
        fn = w & 63
        if fn in (0x08,):  # jr
            return None
        if fn in (0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x0C, 0x0D, 0x0F, 0x11, 0x13):
            return None  # mult/div/syscall/break/sync/mthi/mtlo
        return rd
    if op == 0x1C:  # MMI
        return rd
    if op in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x18, 0x19) or op in LOADS_GPR:
        return rt
    if op == 0x03:
        return 31
    if op in (0x11, 0x12) and rs in (0, 1, 2):  # mfc1/dmfc/cfc1, qmfc2/cfc2
        return rt
    return None


def is_jump(w):
    op = w >> 26
    return op in (2, 3) or (op == 0 and (w & 63) in (8, 9))


def is_branch(w):
    op = w >> 26
    return op in (1, 4, 5, 6, 7, 0x14, 0x15, 0x16, 0x17) or (op in (0x10, 0x11, 0x12) and (w >> 21) & 31 == 8)


def scan_program(p, vram_targets, ok_hi):
    """Yield (kind, address, offset) for each reference to main/BATTLE in program p."""
    end = code_end(p)
    words = [struct.unpack_from("<I", p, a)[0] for a in range(0, len(p) - 3, 4)]
    ncode = end // 4
    for i in range(ncode):
        w = words[i]
        op = w >> 26
        if op in (2, 3):
            t = (w & 0x03FFFFFF) << 2
            if binary_of(t):
                yield ("jal" if op == 3 else "j", t, i * 4)
            continue
        if op == 0x0F and (w >> 21) & 31 == 0:
            hi = (w & 0xFFFF) << 16
            if not ok_hi(hi):
                continue
            found = False
            for a in follow_lui(words, i, ncode):
                found = True
                yield ("hi/lo", a, i * 4)
            if not found:
                yield ("hi only", hi, i * 4)
    for i in range(ncode, len(words)):
        w = words[i]
        if w in vram_targets:
            yield ("pointer", w, i * 4)


def successors(words, j, n):
    """Where execution goes after the control transfer at j and its delay slot
    (None if j isn't one); `call` says whether caller-saved registers die."""
    x = words[j]
    op = x >> 26
    if is_branch(x):
        t = j + 1 + sext16(x & 0xFFFF)
        uncond = op == 4 and (x >> 16) & 0x3FF == 0  # beq $0, $0 (b)
        out = [t] if uncond else [t, j + 2]
        return [k for k in out if 0 <= k < n], False
    if op == 3 or (op == 0 and (x & 63) == 9):  # jal, jalr
        return [j + 2] if j + 2 < n else [], True
    if is_jump(x):  # j (a tail call or a jump in the program), jr
        return [], False
    return None


def apply(words, j, tracked, hi, out):
    """Record the %lo uses of instruction j and return the registers still holding the %hi."""
    x = words[j]
    op = x >> 26
    rs, rt, rd = (x >> 21) & 31, (x >> 16) & 31, (x >> 11) & 31
    tracked = set(tracked)
    if op in MEM_OPS and rs in tracked:
        out.add(hi + sext16(x & 0xFFFF))
        if op in LOADS_GPR:
            tracked.discard(rt)
        return tracked
    if op in (0x09, 0x19) and rs in tracked:  # addiu/daddiu %lo
        out.add(hi + sext16(x & 0xFFFF))
        tracked.discard(rt)
        return tracked
    if op == 0 and (x & 63) in (0x21, 0x2D) and (rs in tracked) != (rt in tracked):
        tracked.add(rd)  # addu/daddu of an index: still %hi-based
        return tracked
    wr = writes(x)
    if wr is not None:
        tracked.discard(wr)
    return tracked


def follow_lui(words, i, n, budget=600):
    """Full addresses formed from the lui at index i: each %lo use of its
    register on any path from it (branches followed, calls clobber the
    caller-saved registers, `jr`/`j` end a path)."""
    w = words[i]
    hi = (w & 0xFFFF) << 16
    start = frozenset({(w >> 16) & 31})
    out = set()
    work = []
    prev = successors(words, i - 1, n) if i > 0 else None
    if prev is not None:  # the lui is a delay slot
        targets, call = prev
        t = start - CALLER_SAVED if call else start
        work = [(k, t) for k in targets]
    else:
        work = [(i + 1, start)]
    seen = set()
    while work and budget > 0:
        j, tracked = work.pop()
        while 0 <= j < n and tracked and budget > 0:
            if (j, tracked) in seen:
                break
            seen.add((j, tracked))
            budget -= 1
            nxt = successors(words, j, n)
            tracked = frozenset(apply(words, j, tracked, hi, out))
            if nxt is None:
                j += 1
                continue
            if j + 1 < n:  # the delay slot
                tracked = frozenset(apply(words, j + 1, tracked, hi, out))
            targets, call = nxt
            if call:
                tracked = tracked - CALLER_SAVED
            if not targets:
                break
            for k in targets[1:]:
                work.append((k, tracked))
            j = targets[0]
    return sorted(out)


# ------------------------------------------------------------------ driver
def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--iso", default=os.path.join(ROOT, "iso"), help="the disc's files (default iso/)")
    ap.add_argument("--write", action="store_true", help="write config/abi_main.txt and config/abi_battle.txt")
    ap.add_argument("-v", action="store_true")
    args = ap.parse_args()

    funcs = {b: functions(b) for b in RANGES}
    names = {b: Names(b) for b in RANGES}
    named = set()
    for b in RANGES:
        named |= funcs[b] | set(names[b].exact)
    his = set()
    for b, (lo, hi) in RANGES.items():
        his |= set(range(lo >> 16, (hi >> 16) + 2))

    def ok_hi(h):
        return h >> 16 in his

    refs = collections.defaultdict(lambda: collections.Counter())  # (binary, addr) -> kind counts
    files = collections.defaultdict(set)
    odd = collections.Counter()
    progs = programs(args.iso)
    for f, entry, p in progs:
        for kind, a, off in scan_program(p, named, ok_hi):
            b = binary_of(a)
            if b is None:
                continue
            if kind in ("jal", "j") and a not in funcs[b]:
                odd[(b, kind + " into a function", a)] += 1
                kind = kind + " (mid)"
            refs[(b, a, kind == "hi only")][kind] += 1
            files[(b, a, kind == "hi only")].add((f, entry))
    # data-only files: absolute pointers to main/BATTLE?
    for f in data_files(args.iso):
        d = open(f, "rb").read()
        for o in range(0, len(d) - 3, 4):
            w = struct.unpack_from("<I", d, o)[0]
            if w in funcs["main"] or w in funcs["battle"]:
                odd[(binary_of(w), "pointer-like word in " + os.path.basename(f), w)] += 1

    print(f"{len(progs)} programs ({sum(1 for f, e, p in progs if f.endswith('.DAT'))} DAT, "
          f"{sum(1 for f, e, p in progs if f.endswith('.PAC'))} GBX entries)")
    report = {}
    for b in ("main", "field", "battle"):
        rows = []
        for (bb, a, hionly), kinds in refs.items():
            if bb != b:
                continue
            is_func = a in funcs[b] and not hionly
            kind = "hi" if hionly else "func" if is_func else "data"
            fs = files[(bb, a, hionly)]
            rows.append((a, kind, names[b].name(a), len({x for x, _ in fs}), len(fs), sum(kinds.values()), kinds))
        rows.sort()
        report[b] = rows
        c = collections.Counter(r[1] for r in rows)
        nref = collections.Counter()
        for r in rows:
            nref[r[1]] += r[5]
        print(f"{b:7}: {c['func']} functions ({nref['func']} refs), {c['data']} data addresses "
              f"({nref['data']} refs), {c['hi']} unpaired upper halves ({nref['hi']} luis)")
    for k, n in sorted(odd.items(), key=lambda x: (x[0][0] or "", x[0][2])):
        print(f"  note: {k[0]} {k[1]} {k[2]:08X} x{n}")

    if args.write:
        for b, path in OUTPUTS.items():
            write_list(b, path, report[b], len(progs))
    if args.v:
        for b in report:
            for r in report[b]:
                print(b, f"{r[0]:08X}", *r[1:])


HEADER = """\
# ABI of the {what}: the {nf} functions and {nd} global addresses that the
# disc's external programs (character programs in SYS/GBX.PAC, magic and
# skill programs in BTL_DATA/*.DAT; {nprog} programs) use by absolute address.
# A relinked {what} must keep each of them at this address (the pinned link,
# tools/pin_link.py; docs/shifting.md "The ABI"). Generated by
# tools/abi_scan.py --write from the user's disc; only addresses and our own
# names are listed. Re-run it after renames.
#
# <address> <func|data> <name>  # <files> files, <programs> programs, <refs> refs (<kinds>)
#   (SYS/GBX.PAC is one file holding many programs)
#   func   a jal/j target (function start)
#   data   an address formed by a lui and its %lo (lui/addiu, lui/lw/sw, ...)
#          or a pointer word in a program's data; name+0xN is inside an object
# Upper halves whose %lo wasn't found are listed at the end as comments.
"""


def write_list(b, path, rows, nprog):
    what = "main executable" if b == "main" else "BATTLE overlay"
    nf = sum(1 for r in rows if r[1] == "func")
    nd = sum(1 for r in rows if r[1] == "data")
    with open(os.path.join(ROOT, path), "w") as f:
        f.write(HEADER.format(what=what, nf=nf, nd=nd, nprog=nprog))
        for kind in ("func", "data"):
            f.write(f"\n# {'functions' if kind == 'func' else 'globals'}\n")
            for a, k, name, nfiles, nprogs, nref, kinds in rows:
                if k != kind:
                    continue
                how = ", ".join(f"{x} {n}" for x, n in sorted(kinds.items()))
                f.write(f"0x{a:08X} {k} {name:<28} # {nfiles} files, {nprogs} programs, {nref} refs ({how})\n")
        his = [r for r in rows if r[1] == "hi"]
        if his:
            f.write("\n# upper halves with no %lo found (not pinned by themselves)\n")
            for a, k, name, nfiles, nprogs, nref, kinds in his:
                f.write(f"# hi 0x{a >> 16:04X} # {nfiles} files, {nprogs} programs, {nref} luis\n")
    print(f"wrote {path}")


if __name__ == "__main__":
    main()
