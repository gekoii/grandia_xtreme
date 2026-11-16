#!/usr/bin/env python3
"""Shift test: relink a binary with padding inserted and check that every
address reference moved with it.

Usage (in the build container, after a normal `make`):
  tools/docker.sh python3 tools/shift_test.py [binary...] [--at POINT...]
      [--together] [--only-together] [--pad 0xN] [--pinned] [--allow-known] [--keep] [-v]

  binary   main, field, battle, viewer (default: all four)
  --at     where to insert padding (default: text data rodata bss), one
           relink per point. text = after the binary's first .text object;
           data / rodata / bss = start of that section (splat's marker
           before its first input; bss includes .scommon where there is
           one); 0xADDR = before the input section that starts at or after
           that address.
  --together  one more relink with padding at all the points at once.
  --pad    base padding (default 0x80). The n-th point gets base << n
           (0x80, 0x100, 0x200, 0x400, ...), so a wrong value shows where it
           came from. Must keep every piece's alignment: a multiple of 0x80.
  --allow-known  references listed as `known` in config/shift_ok.txt (real
           ones that don't shift yet, documented in docs/shifting.md) are
           still reported, but don't make the exit status 1. CI uses this.
  --pinned relink main and BATTLE (default: both) with tools/pin_link.py
           instead: the units holding their ABI (config/abi_*.txt) stay,
           everything else moves unit by unit. Each unit's own offset is
           checked, and every ABI address and name must be where it was.

Every link goes to build/shift/ with --emit-relocs; the normal build is not
touched. For each case the binary is linked twice from the same objects:
the baseline (same layout as the normal build, which is checked byte for
byte) and the padded one. When the main executable is padded, the three
overlays are relinked too, against the padded main ELF.

Checks, per relinked binary:
  bytes  every byte not covered by a relocation is identical at its
         shifted address (and the baseline equals the normal build).
  reloc  every relocated word (R_MIPS_32, R_MIPS_26, HI16/LO16 pairs) points
         at the shifted image of its baseline target. Failures that kept
         their old value are "not moved" (an absolute symbol, listed by
         name); anything else is "wrong".
  raw    words in data, and `lui`s in code, with no relocation whose value
         falls in a shifted range: pointers or %hi halves the build doesn't
         know about (or coincidences: strings, packed u16 pairs, constants).

Results: a summary per case on stdout, details in build/shift/<case>.txt.
Exit status 0 if nothing failed, 1 if only unmoved/raw references were
found, 2 on byte mismatches, wrongly moved references or link errors.
config/shift_ok.txt lists exceptions (see docs/shifting.md): `addr`/`abs`
lines are references meant to stay, which are only counted; `known` lines
are references that really don't shift yet, reported as "known" and counted
as failures unless --allow-known is given.
"""
import argparse
import bisect
import collections
import glob
import os
import re
import struct
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elf32  # noqa: E402
import ld_script  # noqa: E402
import pin_link  # noqa: E402
from binaries import BINARIES  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
OUT = "build/shift"
LD = "mips-linux-gnu-ld"
LDFLAGS = ["-EL", "--no-check-sections"]
ALL = ["main", "field", "battle", "viewer"]
OVERLAYS = ["field", "battle", "viewer"]


def elf_name(b):
    return "SLUS_204.17" if b == "main" else b


def run(cmd):
    r = subprocess.run(cmd, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode:
        sys.stdout.write(r.stdout)
        raise SystemExit(f"command failed: {' '.join(cmd)}")
    return r.stdout


# ------------------------------------------------------------------ linking
def link(b, script, out, csyms):
    """Link binary b like the Makefile does, plus --emit-relocs and a map."""
    cmd = [LD] + LDFLAGS + ["-q", "-Map", out[:-4] + ".map", "-o", out] + ld_script.link_scripts(b, script, csyms)
    run(cmd)


def make_csyms(b, out, main_elf):
    """The binary's c_syms linker script, as the Makefile writes it."""
    run(["python3"] + ld_script.c_syms_args(b, out, main_elf))


map_sections = ld_script.map_sections


# ------------------------------------------------------------------ images
class Image:
    """An ELF's loaded sections, addressable by vram."""

    def __init__(self, path):
        self.elf = elf32.Elf(os.path.join(ROOT, path))
        self.secs = []
        for s in self.elf.alloc_sections():
            self.secs.append((s["addr"], s["addr"] + s["size"], s, self.elf.data(s)))
        self.secs.sort(key=lambda x: x[0])
        self.syms = self.elf.defined()

    def find(self, addr):
        for lo, hi, s, d in self.secs:
            if lo <= addr < hi:
                return lo, d
        return None, None

    def word(self, addr):
        lo, d = self.find(addr)
        if d is None:
            return None
        return struct.unpack_from("<I", d, addr - lo)[0]

    def relocs(self):
        """Emitted relocations by site address, in the order of the reloc sections."""
        return [(off, typ, sym) for off, typ, sym, add, tsec in self.elf.relocs()]


# ------------------------------------------------------------------ cases
class Case:
    """A set of insertion points with padding, per binary, and the mapping
    from baseline addresses to padded ones."""

    def __init__(self, name, pads):
        self.name = name
        self.pads = pads  # binary -> [(point name, size)]
        self.points = {}  # binary -> sorted [(baseline addr, size)]
        self.range = {}  # binary -> (lo, end) its own addresses (end inclusive)

    def resolve(self, b, base):
        """Baseline addresses of the points, from the markers in the baseline link."""
        pts = []
        for pname, size in self.pads.get(b, []):
            sym = base.syms.get(ld_script.point_symbol(pname))
            if sym is None:
                print(f"{b}: no input section starts at point {pname}: not padded")
                continue
            pts.append((sym["value"], size, pname))
        self.points[b] = sorted(pts)
        self.range[b] = binary_range(b, base)

    def m(self, a):
        """Shifted address of baseline address a (bytes at a point move)."""
        for b, pts in self.points.items():
            lo, end = self.range[b]
            if lo <= a <= end:
                return a + sum(size for x, size, _ in pts if x <= a)
        return a

    def targets(self, t):
        """Acceptable shifted values for a reference to baseline address t: a
        pointer exactly at a point may be the end of what precedes it."""
        out = {self.m(t)}
        for b, pts in self.points.items():
            lo, end = self.range[b]
            if lo <= t <= end:
                for x, size, _ in pts:
                    if x == t:
                        out.add(self.m(t) - size)
        return out

    def shifted(self, v):
        """Whether baseline address v moves in this case."""
        return self.m(v) != v or any(v in [x for x, _, _ in pts] for pts in self.points.values())

    def deltas(self):
        """Every offset an address can move by in this case, low 16 bits."""
        out = {0}
        for b, pts in self.points.items():
            total = 0
            for x, size, _ in pts:
                total += size
                out.add(total & 0xFFFF)
        return out

    def shifted_ranges(self):
        out = []
        for b, pts in self.points.items():
            if pts:
                out.append((pts[0][0], self.range[b][1] + 1))
        return out


def binary_range(b, img):
    """(first, last) address of a binary's own memory: its code segment up to
    the end of its .bss (the main executable's 0x700000 stub is fixed)."""
    vram = BINARIES[b]["vram"]
    end = vram
    for lo, hi, s, d in img.secs:
        if b == "main" and lo >= 0x00700000:
            continue
        end = max(end, hi)
    return vram, end


# ------------------------------------------------------------------ checks
def sext16(x):
    return x - 0x10000 if x & 0x8000 else x


WORD_LITERAL_RE = re.compile(r"/\*\s+[0-9A-F]+\s+([0-9A-F]{8})\b[^*]*\*/\s+\.word\s+0x[0-9A-Fa-f]+\s*$")


def word_literals(b):
    """Addresses where the binary's data asm writes a plain `.word 0x...`: data
    splat treats as untyped words. Values written as .short/.byte/strings/
    floats were typed (config/symbol_addrs*.txt), so they're not pointers."""
    out = set()
    for path in glob.glob(os.path.join(ROOT, f"asm/{b}/data/**/*.s"), recursive=True):
        for line in open(path):
            m = WORD_LITERAL_RE.search(line)
            if m:
                out.add(int(m.group(1), 16))
    return out


class Result:
    def __init__(self):
        self.count = collections.Counter()
        self.lines = []
        self.abs_syms = collections.Counter()

    def add(self, kind, text):
        self.count[kind] += 1
        self.lines.append(f"{kind:14} {text}")


def owner_index(sections):
    starts = [s[0] for s in sections]
    return starts, sections


def owner(idx, a):
    starts, secs = idx
    i = bisect.bisect_right(starts, a) - 1
    if i >= 0 and secs[i][0] <= a < secs[i][0] + secs[i][1]:
        return secs[i]
    return None


_MEMORY_MAP = None


def memory_map_names():
    """Names config/memory_map.ld fixes (load addresses, GBX slots, the
    magic program area, ...): they never move, even where a padded binary's
    range covers them (VIEWER's .bss covers the first GBX slots)."""
    global _MEMORY_MAP
    if _MEMORY_MAP is None:
        _MEMORY_MAP = set(re.findall(r"^\s*(\w+)\s*=", open(os.path.join(ROOT, "config/memory_map.ld")).read(), re.M))
    return _MEMORY_MAP


def pair_relocs(rel, word):
    """site -> (type, symbol, paired site) for a list of (site, type, symbol).
    gas puts each HI16 right before the LO16 it pairs with, so that pairing
    comes first. A HI16 not followed by a LO16 of its symbol then pairs with
    the next unused LO16 of its symbol whose base register is the lui's, and
    failing that with the next unused LO16 of its symbol (ld -q gives every
    section-symbol relocation the output section's symbol, so "its symbol" is
    weak there). Other LO16s share an earlier %hi and are checked on their
    low half only."""
    sites = {}
    his = []
    for i, (off, typ, sym) in enumerate(rel):
        if off not in sites:
            sites[off] = (typ, sym, None)
        if typ == elf32.R_MIPS_HI16:
            his.append(i)
    taken = set()
    for pass_ in (0, 1, 2):
        for i in his:
            off, typ, sym = rel[i]
            if sites[off][2] is not None:
                continue
            rt = (word(off) >> 16) & 31
            window = rel[i + 1:i + 2] if pass_ == 0 else rel[i + 1:i + 64]
            for o2, t2, s2 in window:
                if t2 == elf32.R_MIPS_LO16 and s2["index"] == sym["index"] and o2 not in taken \
                        and (pass_ != 1 or (word(o2) >> 21) & 31 == rt):
                    taken.add(o2)
                    sites[off] = (typ, sym, o2)
                    sites[o2] = (t2, s2, off)
                    break
    return sites


def check_binary(b, case, base, pad, base_map, res, ok):
    """Compare binary b's baseline and padded links under case's mapping."""
    m = case.m
    own = case.points.get(b, [])
    idx = owner_index(base_map)

    # Relocations. gas puts each HI16 right before the LO16 it pairs with
    # (or, failing that, before a later LO16 of the same symbol); other LO16s
    # share an earlier %hi and are checked on their low half only.
    brel = base.relocs()
    prel = {off: (typ, sym) for off, typ, sym in pad.relocs()}
    sites = pair_relocs(brel, base.word)

    def target(img, site, mapped):
        typ, sym, other = sites[site]
        f = (lambda a: m(a)) if mapped else (lambda a: a)
        w = img.word(f(site))
        if typ == elf32.R_MIPS_32:
            return w
        if typ == elf32.R_MIPS_26:
            return (f(site) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
        if typ == elf32.R_MIPS_HI16:
            if other is None:
                return None
            return ((w & 0xFFFF) << 16) + sext16(img.word(f(other)) & 0xFFFF)
        if typ == elf32.R_MIPS_LO16:
            if other is None:
                return None
            return ((img.word(f(other)) & 0xFFFF) << 16) + sext16(w & 0xFFFF)
        return None

    def where(a):
        o = owner(idx, a)
        return f"{o[3]}:{o[2]}" if o else "?"

    for site, (typ, sym, other) in sites.items():
        rname = elf32.RELOC_NAMES.get(typ, str(typ))
        sname = sym["name"] or "<section>"
        if m(site) not in prel:
            res.add("bytes", f"{site:08X} relocation {rname} {sname} has no counterpart at {m(site):08X}")
            continue
        res.count["reloc-" + rname] += 1
        if typ == elf32.R_MIPS_LO16 and other is not None:
            continue  # checked with its HI16
        absolute = sym["shndx"] == elf32.SHN_ABS
        if typ == elf32.R_MIPS_LO16:
            # A %lo sharing an earlier %hi: its own target is unknown, so an
            # absolute symbol is judged by its value, anything else by
            # whether its low half moved by one of this case's offsets.
            lb = base.word(site) & 0xFFFF
            lp = pad.word(m(site)) & 0xFFFF
            if absolute:
                tb = sym["value"]
                ps = pad.syms.get(sym["name"])
                tp = ps["value"] if ps else tb
                if tp in case.targets(tb):
                    continue
                kind = "not-moved" if tp == tb else "wrong"
            else:
                if (lp - lb) & 0xFFFF in case.deltas():
                    continue
                res.add("wrong", f"{site:08X} LO16 low half {lb:04X} now {lp:04X} sym {sname} in {where(site)}")
                continue
        else:
            tb, tp = target(base, site, False), target(pad, site, True)
            if tb is None:
                res.count["reloc-undecoded"] += 1
                continue
            if tp in case.targets(tb):
                continue
            kind = "not-moved" if tp == tb else "wrong"
        if absolute and sname in memory_map_names() and tp == tb:
            res.count["ok-memory-map"] += 1  # fixed by design (config/memory_map.ld)
            continue
        if (b, "abs", sname) in ok or (b, "addr", tb) in ok:
            res.count["ok-" + kind] += 1
            continue
        if kind == "not-moved" and (b, "known", tb) in ok:
            kind = "known"
        res.add(kind, f"{site:08X} {rname:4} -> {tb:08X} now {tp:08X} expected {case.m(tb):08X}"
                      f" sym {sname}{' (absolute)' if absolute else ''} in {where(site)}")
        if kind != "known" and (absolute or kind == "not-moved"):
            res.abs_syms[(sname, tb)] += 1

    # Bytes: everything else must be identical at its shifted address. A
    # case that moves units one by one (a pinned link) gives the units to
    # compare; otherwise each section is cut at the padding points.
    site_list = sorted(sites)
    chunks = case.chunks(b) if hasattr(case, "chunks") else None
    if chunks is None:
        chunks = []
        bounds = sorted({x for x, _, _ in own})
        for lo, hi, s, d in base.secs:
            if s["type"] == elf32.SHT_NOBITS:
                continue
            cuts = [lo] + [x for x in bounds if lo < x < hi] + [hi]
            chunks += list(zip(cuts, cuts[1:]))
    for a0, a1 in chunks:
        lo, d = base.find(a0)
        if d is None:
            continue  # zero-initialized (NOBITS): nothing to compare
        plo, pd = pad.find(m(a0))
        if pd is None:
            res.add("bytes", f"{a0:08X}-{a1:08X} missing in the padded link")
            continue
        bb = bytearray(d[a0 - lo:a1 - lo])
        pb = bytearray(pd[m(a0) - plo:m(a0) - plo + (a1 - a0)])
        for site in site_list[bisect.bisect_left(site_list, a0 - 3):bisect.bisect_left(site_list, a1)]:
            o = site - a0
            if o < 0:
                bb[0:o + 4] = pb[0:o + 4] = bytes(o + 4)
            else:
                bb[o:o + 4] = pb[o:o + 4] = b"\0\0\0\0"
        if bb != pb:
            bad = [i for i in range(min(len(bb), len(pb))) if i >= len(pb) or bb[i] != pb[i]]
            if len(bb) != len(pb):
                bad.append(min(len(bb), len(pb)))
            for i in bad[:20]:
                res.add("bytes", f"{a0 + i:08X} differs (baseline {bb[i] if i < len(bb) else -1:02X} padded "
                                 f"{pb[i] if i < len(pb) else -1:02X}) in {where(a0 + i)}")
            if len(bad) > 20:
                res.add("bytes", f"... {len(bad) - 20} more differing bytes in {a0:08X}-{a1:08X}")

    # Raw values: unrelocated data words and lui halves in shifted ranges.
    ranges = case.shifted_ranges()
    literals = word_literals(b)

    def in_shifted(v):
        if hasattr(case, "in_shifted"):
            return case.in_shifted(v)
        return any(lo <= v < hi for lo, hi in ranges)

    for a, size, sec, obj in base_map:
        if a >= 0x00700000 and b == "main":
            continue
        is_code = sec.startswith(".text") and "/data/" not in obj
        is_vu = sec.startswith(".text") and "/data/" in obj
        if sec.startswith(".bss") or sec in (".scommon", "COMMON"):
            continue
        a0 = (a + 3) & ~3
        for w_at in range(a0, a + size - 3, 4):
            if w_at in sites:
                continue
            w = base.word(w_at)
            if w is None:
                continue
            if is_code:
                if w >> 26 == 0x0F and (w >> 21) & 31 == 0:
                    hi = (w & 0xFFFF) << 16
                    if in_shifted(hi) or in_shifted(hi - 0x8000) or in_shifted(hi + 0x7FFF):
                        if (b, "addr", w_at) in ok:
                            res.count["ok-raw"] += 1
                            continue
                        kind = "raw-lui-c" if obj.startswith("build/src/") else "raw-lui-asm"
                        if kind == "raw-lui-asm" and (b, "known", w_at) in ok:
                            kind = "known"
                        res.add(kind, f"{w_at:08X} lui {w & 0xFFFF:#06x} in {obj}")
                elif w >> 26 in (2, 3):
                    t = (w_at & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
                    if in_shifted(t):
                        res.add("raw-jump", f"{w_at:08X} {'jal' if w >> 26 == 3 else 'j'} {t:08X} in {obj}")
                continue
            if not in_shifted(w):
                continue
            if (b, "addr", w_at) in ok:
                res.count["ok-raw"] += 1
                continue
            if is_vu:
                kind = "raw-word-vu"
            elif obj.startswith("build/src/"):
                kind = "raw-word-c"  # a C file's constant
            elif w_at in literals:
                kind = "known" if (b, "known", w_at) in ok else "raw-word"
            else:
                kind = "raw-word-typed"
            res.add(kind, f"{w_at:08X} .word {w:08X} in {obj}:{sec}")


def read_ok(path):
    """config/shift_ok.txt: `<binary> abs <symbol>` or `<binary> addr <address>`
    (or `<start>-<end>`, every word in it) lines, each with a # comment saying
    why the reference is meant to stay; `<binary> known <address>` (or a
    range) for a reference that doesn't shift and should, kept as a known
    open problem (see --allow-known)."""
    ok = set()
    if not os.path.exists(os.path.join(ROOT, path)):
        return ok
    for line in open(os.path.join(ROOT, path)):
        line = line.split("#", 1)[0].split()
        if len(line) < 3:
            continue
        b, kind, what = line[:3]
        if kind not in ("abs", "addr", "known"):
            raise SystemExit(f"{path}: unknown kind {kind!r} in {' '.join(line)}")
        if kind in ("addr", "known") and "-" in what:
            lo, hi = (int(x, 16) for x in what.split("-"))
            ok.update((b, kind, a) for a in range(lo, hi))
            continue
        if kind in ("addr", "known"):
            what = int(what, 16)
        ok.add((b, kind, what))
    return ok


# ------------------------------------------------------------------ pinned links
class MapCase(Case):
    """A case in which some binaries were relinked unit by unit with
    tools/pin_link.py: each input section (unit) moves by its own offset.
    Binaries padded the plain way keep Case's point mapping."""

    def __init__(self, name, pads):
        super().__init__(name, pads)
        self.units = {}  # binary -> (starts, [(start, end, delta)])

    def set_units(self, b, units, base):
        sized = sorted((u.addr, u.addr + u.size, u.new - u.addr) for u in units
                       if u.size and u.outsec != ".lay_subprog1")
        self.units[b] = ([x[0] for x in sized], sized)
        self.points[b] = []
        self.range[b] = binary_range(b, base)

    def _find(self, a):
        """(units, index) of the unit holding a, or of the one before it if a is
        in the fill after a unit; None if a isn't in a unit-mapped binary."""
        for b, (starts, sized) in self.units.items():
            lo, end = self.range[b]
            if lo <= a <= end:
                return sized, max(bisect.bisect_right(starts, a) - 1, 0)
        return None

    def m(self, a):
        f = self._find(a)
        if f is None:
            return super().m(a)
        sized, i = f
        return a + sized[i][2]

    def targets(self, t):
        f = self._find(t)
        if f is None:
            return super().targets(t)
        sized, i = f
        out = {t + sized[i][2]}
        if sized[i][0] == t and i > 0:
            out.add(t + sized[i - 1][2])  # the end of the unit before it
        return out

    def shifted(self, v):
        return self.m(v) != v

    def in_shifted(self, v):
        f = self._find(v)
        if f is None:
            return any(lo <= v < hi for lo, hi in super().shifted_ranges())
        sized, i = f
        return sized[i][2] != 0

    def deltas(self):
        out = super().deltas()
        for starts, sized in self.units.values():
            out |= {d & 0xFFFF for _, _, d in sized}
        return out

    def chunks(self, b):
        if b not in self.units:
            return None
        return [(s, e) for s, e, d in self.units[b][1]]


def check_abi(b, units, pad, normal, res):
    """Every ABI address of b (config/abi_<b>.txt) is where it was, and so is
    every name the list gives."""
    sized = sorted((u.addr, u.addr + u.size, u) for u in units if u.size)
    starts = [x[0] for x in sized]
    for a, name in pin_link.abi_addresses(b):
        i = bisect.bisect_right(starts, a) - 1
        u = sized[i][2]
        if not (u.addr <= a < u.addr + u.size) or u.new != u.addr:
            res.add("abi", f"{a:08X} {name} moved to {a + u.new - u.addr:08X}")
            continue
        res.count["abi-ok"] += 1
        sym = name.split("+")[0]
        ns, ps = normal.syms.get(sym), pad.syms.get(sym)
        if ns and ps and ns["value"] != ps["value"]:
            res.add("abi", f"{sym} {ns['value']:08X} now {ps['value']:08X}")


def same_as_normal(b, img, what):
    """A baseline link must give the normal build's loaded bytes and symbols."""
    normal = Image(f"build/{elf_name(b)}.elf")
    for lo, hi, s, d in normal.secs:
        if s["type"] == elf32.SHT_NOBITS:
            continue
        plo, pd = img.find(lo)
        if pd is None or pd[lo - plo:hi - plo] != d:
            raise SystemExit(f"{b}: the {what} differs from build/{elf_name(b)}.elf in {s['name']}")
    moved = [n for n, sy in normal.syms.items()
             if n in img.syms and img.syms[n]["value"] != sy["value"] and not n.startswith(("__shift", "$"))
             and sy["shndx"] != elf32.SHN_ABS]
    if moved:
        raise SystemExit(f"{b}: the {what} moves {len(moved)} symbols, e.g. {moved[:5]}")
    return normal


class Linker:
    """Links the binaries of a case: plain (splat's script with padding, as
    the Makefile links) or pinned (tools/pin_link.py)."""

    def __init__(self, points, out=OUT):
        self.points = points
        self.out = out
        self.base = {}  # (binary, "plain"|"pin") -> Image
        self.base_map = {}
        self.base_units = {}

    def baseline(self, b, kind):
        key = (b, kind)
        if key in self.base:
            return
        main_elf = None
        if b != "main":
            mk = ("main", "pin") if ("main", "pin") in self.base else ("main", "plain")
            if mk not in self.base:
                self.baseline("main", "plain")
            main_elf = self.base[mk].elf.path
        if kind == "pin":
            us, elf, end = pin_link.pinned_link(b, {}, f"{self.out}/{b}.pinbase", main_elf)
            self.base_units[b] = us
            self.base[key] = Image(elf)
            same_as_normal(b, self.base[key], "pinned baseline")
        else:
            script = f"{self.out}/{b}.base.ld"
            ld_script.write_script(b, script, points=self.points, pads={})
            csyms = ld_script.c_syms_path(b)
            if b != "main":
                csyms = f"{self.out}/c_syms.{b}.base.ld"
                make_csyms(b, csyms, main_elf)
            out = f"{self.out}/{b}.base.elf"
            link(b, script, out, csyms)
            self.base[key] = Image(out)
            same_as_normal(b, self.base[key], "baseline link (the harness no longer links like the Makefile)")
        self.base_map[key] = map_sections(self.base[key].elf.path[:-4] + ".map")

    def run(self, name, spec, ok, allow_known):
        """Link and check one case. spec: {binary: ("pad", [(point, size)]) or
        ("pin", {point: size})}; the overlays are relinked when main moves.
        Returns ({binary: Result}, {binary: elf path}, case)."""
        relink = [b for b in ALL if b in spec or ("main" in spec and b in OVERLAYS)]
        kinds = {b: spec[b][0] if b in spec else "plain" for b in relink}
        kinds = {b: "pin" if k == "pin" else "plain" for b, k in kinds.items()}
        for b in relink:
            self.baseline(b, kinds[b])
        case = MapCase(name, {b: v[1] for b, v in spec.items() if v[0] == "pad"})
        for b in relink:
            if kinds[b] == "plain":
                case.resolve(b, self.base[(b, "plain")])
        results, elfs = {}, {}
        main_elf = None
        for b in relink:
            base = self.base[(b, kinds[b])]
            out = f"{self.out}/{name}.{b}.elf"
            if b != "main":
                main_base = self.base.get(("main", "pin")) or self.base[("main", "plain")]
                main_elf = elfs.get("main") or main_base.elf.path
            if kinds[b] == "pin":
                pads = spec[b][1] if b in spec else {}
                us, elf, end = pin_link.pinned_link(b, pads, out[:-4], main_elf)
                case.set_units(b, us, base)
            else:
                pads = dict(spec[b][1]) if b in spec and spec[b][0] == "pad" else {}
                script = out[:-4] + ".ld"
                ld_script.write_script(b, script, points=self.points, pads=pads)
                csyms = ld_script.c_syms_path(b)
                if b != "main":
                    csyms = f"{self.out}/c_syms.{name}.{b}.ld"
                    make_csyms(b, csyms, main_elf)
                link(b, script, out, csyms)
                us = None
            elfs[b] = out
            res = Result()
            pad_img = Image(out)
            check_binary(b, case, base, pad_img, self.base_map[(b, kinds[b])], res, ok)
            if kinds[b] == "pin":
                check_abi(b, us, pad_img, Image(f"build/{elf_name(b)}.elf"), res)
            results[b] = res
        return results, elfs, case


def summarize(name, b, res, case, allow_known):
    """(status 0/1/2, summary line) for one binary of a case."""
    c = res.count
    bad = c["bytes"] + c["wrong"] + c["raw-jump"] + c["abi"]
    # raw-lui-c is reported but doesn't count: so far every one is a
    # constant (flags, GS register values, sizes), see docs/shifting.md
    soft = c["not-moved"] + c["raw-lui-asm"] + c["raw-word"]
    if not allow_known:
        soft += c["known"]
    nrel = sum(v for k, v in c.items() if k.startswith("reloc-") and k != "reloc-undecoded")
    if b in getattr(case, "units", {}):
        sized = case.units[b][1]
        pts = f"pinned: {sum(1 for _, _, d in sized if d)} of {len(sized)} units moved"
    else:
        pts = ", ".join(f"{p}@{x:08X}+{s:#x}" for x, s, p in case.points.get(b, [])) or "not padded"
    line = (f"{name:14} {b:7} [{pts}] relocs {nrel}: not-moved {c['not-moved']} "
            f"({len(res.abs_syms)} symbols), wrong {c['wrong']}; bytes {c['bytes']}; "
            f"raw words {c['raw-word']} (+{c['raw-word-typed']} typed, {c['raw-word-c']} C, {c['raw-word-vu']} VU); "
            f"raw lui asm {c['raw-lui-asm']} C {c['raw-lui-c']}")
    if c["abi-ok"] or c["abi"]:
        line += f"; ABI {c['abi-ok']} in place, {c['abi']} moved"
    if c["known"]:
        line += f"; known {c['known']}{' (allowed)' if allow_known else ''}"
    if c["ok-not-moved"] or c["ok-raw"] or c["ok-wrong"]:
        line += f"; listed in shift_ok {c['ok-not-moved'] + c['ok-raw'] + c['ok-wrong']}"
    return (2 if bad else 1 if soft else 0), line


def write_details(path, results):
    with open(path, "w") as f:
        for b, res in results.items():
            f.write(f"== {b}\n")
            for (sname, tb), n in sorted(res.abs_syms.items(), key=lambda x: x[0][1]):
                f.write(f"unmoved-symbol {sname} {tb:08X} x{n}\n")
            f.write("\n".join(res.lines) + "\n")


# ------------------------------------------------------------------ driver
DEFAULT_POINTS = ["text", "data", "rodata", "bss"]


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("binaries", nargs="*", default=None)
    ap.add_argument("--at", nargs="+", default=DEFAULT_POINTS)
    ap.add_argument("--together", action="store_true")
    ap.add_argument("--only-together", action="store_true", help="only the case with all points padded")
    ap.add_argument("--pad", type=lambda s: int(s, 0), default=0x80)
    ap.add_argument("--pinned", action="store_true",
                    help="pinned links (tools/pin_link.py) of main and BATTLE: the ABI stays, the rest moves")
    ap.add_argument("--allow-known", action="store_true",
                    help="don't fail on references listed as `known` in config/shift_ok.txt")
    ap.add_argument("--keep", action="store_true", help="keep the ELFs and scripts in build/shift/")
    ap.add_argument("-v", action="store_true", help="print every finding, not only the summary")
    args = ap.parse_args()
    if args.pad % 0x80:
        raise SystemExit("--pad must be a multiple of 0x80")
    binaries = args.binaries or (["main", "battle"] if args.pinned else ALL)
    if args.pinned and set(binaries) - set(pin_link.LIMITS):
        raise SystemExit("--pinned is for main and battle")
    os.chdir(ROOT)
    os.makedirs(OUT, exist_ok=True)
    ok = read_ok("config/shift_ok.txt")

    point_pad = {p: args.pad << i for i, p in enumerate(args.at)}
    cases = []
    tag = "pin." if args.pinned else ""
    for b in binaries:
        def spec(points):
            if args.pinned:
                return {b: ("pin", {p: point_pad[p] for p in points})}
            return {b: ("pad", [(p, point_pad[p]) for p in points])}
        for p in args.at if not args.only_together else []:
            cases.append((f"{tag}{b}.{p}", spec([p])))
        if args.together or args.only_together:
            cases.append((f"{tag}{b}.all", spec(args.at)))

    linker = Linker(args.at)
    worst = 0
    summary = []
    for name, spec in cases:
        results, elfs, case = linker.run(name, spec, ok, args.allow_known)
        if not args.pinned and not any(case.points.values()):
            print(f"{name}: nothing to pad, skipped")
            continue
        write_details(f"{OUT}/{name}.txt", results)
        for b, res in results.items():
            status, line = summarize(name, b, res, case, args.allow_known)
            worst = max(worst, status)
            summary.append(line)
            print(line, flush=True)
            if args.v:
                for l in res.lines:
                    print("   ", l)
        if not args.keep:
            for b in results:
                for ext in (".elf", ".map", ".ld", ".layout", ".c_syms.ld"):
                    p = f"{OUT}/{name}.{b}{ext}"
                    if os.path.exists(p):
                        os.remove(p)
    with open(f"{OUT}/summary{'.pinned' if args.pinned else ''}.txt", "w") as f:
        f.write("\n".join(summary) + "\n")
    sys.exit(worst)


if __name__ == "__main__":
    main()
