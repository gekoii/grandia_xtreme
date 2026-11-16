#!/usr/bin/env python3
"""Pinned link: relink the main executable or BATTLE with padding while every
address of their ABI stays where it is.

Usage (in the build container, after a normal `make`):
  tools/docker.sh python3 tools/pin_link.py <main|battle> [--pad POINT=SIZE ...]
      [--main-elf ELF] [--out PREFIX]

  --pad   text=0x400, data=0x80, rodata=0x100, bss=0x200 or 0xADDR=SIZE
          (the same points as tools/shift_test.py); sizes must be multiples
          of 0x80. No --pad gives the baseline: the original layout, which
          must equal the normal build.
  --main-elf  for battle: the main executable to take main names from
          (default build/SLUS_204.17.elf).
  --out   output prefix (default build/pin/<binary>): writes PREFIX.ld,
          PREFIX.elf (linked with --emit-relocs), PREFIX.map, PREFIX.layout.

Why: the disc's character and magic programs (SYS/GBX.PAC, BTL_DATA/*.DAT)
call main and BATTLE functions and use their globals by absolute address
(config/abi_main.txt, config/abi_battle.txt, written by tools/abi_scan.py).
A plain shift moves those addresses and battles crash. Here the units that
hold an ABI address stay at their original addresses and everything else
flows around them (docs/shifting.md, "The pinned link").

Units and placement:
  * A unit is one input section of the normal link: a C file's or asm
    piece's .text, .data or .rodata. The .bss C files (one object each) are
    reassembled for this link with every object in a section of its own
    (build/pin/<binary>/bss_*.s, from the C file's declarations), so a
    pinned global doesn't pin the whole .bss.
  * Pinned: every unit holding an ABI address, and the units with a `known`
    reference in config/shift_ok.txt (references that don't follow a move).
  * Padding goes in at the points, as in the shift test. Every other unit is
    placed, in the original order, at the lowest free address at or after
    the first padded point that keeps its address modulo 0x80 (like the
    shift test's multiples of 0x80: alignment, cache lines and DMA
    boundaries stay as they were) and doesn't overlap a pinned unit. Code
    (.text below .vutext) may go into any such gap; every other unit also
    stays above the units placed before it, so data keeps its order and
    stays next to its neighbours except where a pinned unit is in the way
    (what no longer fits before it goes after it, with everything that
    follows). splat's object boundaries are guesses and code can reach a
    neighbour through a pointer or an index that no relocation shows, so
    data is never reordered to fill gaps (that broke the first
    shift_all_pinned image: docs/runtime_test.md).
  * Units that must move together (together()): a reference symbol+offset
    into another unit, from the relocations of all four binaries, the
    c_syms PROVIDEs, and accesses through pointers that no relocation shows
    (tools/ptr_scan.py: a pointer kept in a register and used at constant
    offsets, passed to a function that does, or passed to memset/MemSet/
    memcpy with a constant size).
  * Initialized units must stay out of the zero-initialized area:
      main    crt0 clears from ptySkillTable (the first .bss object above
              .scommon, splat's name for _fbss) to 0x750000. Initialized
              units that don't fit below .scommon go between the pinned
              .scommon objects and the first pinned .bss object, and
              ptySkillTable (the clear start) and every .bss object that was
              above it are placed above them. Everything below the clear
              start is in the ELF's file image (zeros for .scommon), so no
              object depends on what the loader leaves in memory.
      BATTLE  initialized units go below .scommon while they fit; the rest
              go after the last pinned .bss object.
  * Limits (config/memory_map.ld): main must end below FIELD_VRAM
    (0x005B0000, 0x94E0 of headroom), BATTLE below btlDbgStage (0x006FFFFC,
    0x6CAC). The link fails otherwise.
"""
import argparse
import bisect
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elf32  # noqa: E402
import ld_script  # noqa: E402
import ptr_scan  # noqa: E402
from binaries import BINARIES  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
PIN = "build/pin"
LD = "mips-linux-gnu-ld"
AS = "mips-linux-gnu-as"
ASFLAGS = ["-EL", "-march=r5900", "-mabi=eabi", "-G0", "-no-pad-sections"]
RESIDUE = 0x80
LIMITS = {"main": 0x005B0000, "battle": 0x006FFFFC}  # FIELD_VRAM, btlDbgStage
OUTSEC = {"main": ".main", "battle": ".battle"}
# main: crt0's clear start (_fbss) is the address of this object
MAIN_FBSS = "ptySkillTable"


def run(cmd):
    r = subprocess.run(cmd, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode:
        sys.stdout.write(r.stdout)
        raise SystemExit(f"command failed: {' '.join(cmd)}")
    return r.stdout


# ------------------------------------------------------------------ units
class Unit:
    __slots__ = ("addr", "size", "sec", "obj", "nobits", "outsec", "pinned", "new", "why")

    def __init__(self, addr, size, sec, obj, nobits, outsec):
        self.addr, self.size, self.sec, self.obj, self.nobits, self.outsec = addr, size, sec, obj, nobits, outsec
        self.pinned = False
        self.new = None
        self.why = ""

    @property
    def end(self):
        return self.addr + self.size

    def key(self):
        return (self.obj, self.sec)

    def __repr__(self):
        return f"<{self.obj}({self.sec}) {self.addr:08X}+{self.size:X}>"


MAP_LINE = re.compile(r"^ (\.[\w.$]+)?\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+\.o)\s*$")
OUT_LINE = re.compile(r"^(\.[\w.$]+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)")


def map_units(path):
    """Every input section in a linker map's memory map (zero-sized ones too),
    with its output section, in map order."""
    out = []
    pending = None
    outsec = None
    started = False
    for line in open(os.path.join(ROOT, path)):
        if line.startswith("Linker script and memory map"):
            started = True
            continue
        if not started:
            continue
        if line.startswith("/DISCARD/"):
            break
        m = OUT_LINE.match(line)
        if m:
            outsec = m.group(1)
            pending = None
            continue
        m = re.match(r"^ (\.[\w.$]+)\s*$", line)
        if m:
            pending = m.group(1)
            continue
        m = MAP_LINE.match(line)
        if m and outsec:
            sec = m.group(1) or pending
            pending = None
            out.append(Unit(int(m.group(2), 16), int(m.group(3), 16), sec, m.group(4),
                            sec.startswith((".bss", ".scommon", ".sbss")) or sec == "COMMON", outsec))
            continue
        pending = None
    return out


BSS_DECL = re.compile(r"^u(8|16|32) (\w+)(?:\[(0x[0-9A-Fa-f]+)\])? BSS(?: ALIGN\((\d+)\))?; /\* ([0-9A-F]{8})")


def bss_objects(c_path):
    """(address, size, name, alignment) of every object in a .bss C file
    (tools/gen_bss.py's format)."""
    out = []
    for line in open(os.path.join(ROOT, c_path)):
        m = BSS_DECL.match(line)
        if m:
            width = int(m.group(1)) // 8
            n = int(m.group(3), 16) if m.group(3) else 1
            align = int(m.group(4)) if m.group(4) else width
            out.append((int(m.group(5), 16), width * n, m.group(2), align))
    return out


def bss_split(b):
    """Write and assemble build/pin/<b>/bss_*.s: each .bss C file's objects, one
    section each (.bss.<name>). Returns {C object path: [(addr, size, name)]}."""
    out = {}
    src = f"src/{b}"
    for f in sorted(os.listdir(os.path.join(ROOT, src))):
        if not (f.startswith("bss_") and f.endswith(".c")):
            continue
        stem = f[:-2]
        objs = bss_objects(f"{src}/{f}")
        s_path = f"{PIN}/{b}/{stem}.s"
        o_path = f"{PIN}/{b}/{stem}.o"
        lines = [f"# {src}/{f} with each object in a section of its own (tools/pin_link.py)"]
        for addr, size, name, align in objs:
            lines += [f'.section .bss.{name},"aw",@nobits', f".balign {align}", f".globl {name}",
                      f".type {name}, @object", f".size {name}, {size:#x}", f"{name}:  # {addr:08X}",
                      f"    .space {size:#x}"]
        os.makedirs(os.path.join(ROOT, PIN, b), exist_ok=True)
        text = "\n".join(lines) + "\n"
        full = os.path.join(ROOT, s_path)
        if not os.path.exists(full) or open(full).read() != text or not os.path.exists(os.path.join(ROOT, o_path)):
            open(full, "w").write(text)
            run([AS] + ASFLAGS + ["-o", o_path, s_path])
        out[f"build/src/{b}/{stem}.o"] = (o_path, objs)
    return out


def units(b):
    """The units of binary b: the normal link's input sections, with the .bss
    C files replaced by their objects."""
    split = bss_split(b)
    out = []
    for u in map_units(f"build/{ld_script.elf_name(b)}.map"):
        if u.obj in split and u.sec.startswith(".bss"):
            o_path, objs = split[u.obj]
            if u.size:
                if objs[0][0] != u.addr or objs[-1][0] + objs[-1][1] > u.end:
                    raise SystemExit(f"{u.obj}: its objects don't match the link ({u.addr:08X})")
                for addr, size, name, align in objs:
                    out.append(Unit(addr, size, f".bss.{name}", o_path, True, u.outsec))
            continue
        out.append(u)
    return out


# ------------------------------------------------------------------ pins
def abi_addresses(b):
    out = []
    for line in open(os.path.join(ROOT, f"config/abi_{b}.txt")):
        if line.startswith("0x"):
            f = line.split()
            out.append((int(f[0], 16), f[2]))
    return out


def known_addresses(b):
    """`known` lines of config/shift_ok.txt: references that don't follow a move."""
    out = []
    for line in open(os.path.join(ROOT, "config/shift_ok.txt")):
        f = line.split("#", 1)[0].split()
        if len(f) >= 3 and f[0] == b and f[1] == "known":
            lo, _, hi = f[2].partition("-")
            out.append((int(lo, 16), f"known {f[2]}"))
    return out


def mark_pins(b, us):
    sized = sorted((u.addr, u.end, u) for u in us if u.size)
    starts = [x[0] for x in sized]
    missing = []
    for a, why in abi_addresses(b) + known_addresses(b):
        i = bisect.bisect_right(starts, a) - 1
        if i >= 0 and sized[i][0] <= a < sized[i][1]:
            u = sized[i][2]
            u.pinned = True
            u.why = u.why or why
        else:
            missing.append(a)
    if missing:
        raise SystemExit(f"{b}: ABI addresses in no unit: {' '.join(f'{a:08X}' for a in missing)}")


# ------------------------------------------------------------------ units that move together
def reference_elfs():
    """Every binary linked as the Makefile links it, plus --emit-relocs
    (build/pin/ref.<binary>.elf); relinked when the normal build is newer."""
    out = {}
    for b in ("main", "field", "battle", "viewer"):
        normal = os.path.join(ROOT, f"build/{ld_script.elf_name(b)}.elf")
        elf = f"{PIN}/ref.{b}.elf"
        full = os.path.join(ROOT, elf)
        if not os.path.exists(full) or os.path.getmtime(full) < os.path.getmtime(normal):
            script = f"{PIN}/ref.{b}.ld"
            os.makedirs(os.path.join(ROOT, PIN), exist_ok=True)
            ld_script.write_script(b, script)
            link(b, script, elf, ld_script.c_syms_path(b))
        out[b] = elf
    return out


def sext16(x):
    return x - 0x10000 if x & 0x8000 else x


def reloc_refs(path):
    """(symbol value, target address, symbol name) of every R_MIPS_32 and paired
    HI16/LO16 relocation against a named symbol in a linked ELF (the target
    includes the addend)."""
    e = elf32.Elf(os.path.join(ROOT, path))
    secs = sorted((s["addr"], s["addr"] + s["size"], e.data(s)) for s in e.alloc_sections())
    starts = [x[0] for x in secs]

    def word(a):
        i = bisect.bisect_right(starts, a) - 1
        lo, hi, d = secs[i]
        return int.from_bytes(d[a - lo:a - lo + 4], "little")
    rel = [(off, typ, sym) for off, typ, sym, add, tname in e.relocs()]
    import shift_test  # noqa: E402 (shift_test imports this module)
    sites = shift_test.pair_relocs(rel, word)
    out = []
    for off, typ, sym in rel:
        if not sym["name"] or sym["type"] == 3 or sym["shndx"] == elf32.SHN_UNDEF:
            continue
        if typ == elf32.R_MIPS_32:
            out.append((sym["value"], word(off), sym["name"]))
        elif typ == elf32.R_MIPS_HI16 and sites[off][2] is not None:
            t = ((word(off) & 0xFFFF) << 16) + sext16(word(sites[off][2]) & 0xFFFF)
            out.append((sym["value"], t & 0xFFFFFFFF, sym["name"]))
    return out


PROVIDE_RE = re.compile(r"^PROVIDE\((\S+) = ([\w.$]+)(?: ([+-]) (0x[0-9A-Fa-f]+|\d+))?\);")


def provide_refs(path, syms):
    """(base value, target) of each PROVIDE(name = base + off) in a c_syms script."""
    out = []
    for line in open(os.path.join(ROOT, path)):
        m = PROVIDE_RE.match(line)
        if m and m.group(2) in syms:
            off = int(m.group(4), 0) if m.group(4) else 0
            base = syms[m.group(2)]
            out.append((base, base - off if m.group(3) == "-" else base + off, m.group(1)))
    return out


TOGETHER_VERSION = 3  # 2, 3: with tools/ptr_scan.py's pointer accesses
BIASED = 0x10000  # a reference this far from its symbol is a biased base (config/symbol_exprs.ld)


def together(b):
    """Address ranges [lo, hi] of b whose units must keep their relative
    places: a reference symbol+offset whose target lies in another unit than
    the symbol (a struct that the .bss C file splits into several objects,
    an end pointer, a table indexed past a label). Cached in
    build/pin/<binary>.together.json."""
    refs = reference_elfs()
    cache = os.path.join(ROOT, PIN, f"{b}.together.json")
    stamp = [os.path.getmtime(os.path.join(ROOT, p)) for p in refs.values()]
    if os.path.exists(cache):
        c = json.load(open(cache))
        if c.get("stamp") == stamp and c.get("version") == TOGETHER_VERSION:
            return [tuple(x) for x in c["ranges"]]
    lo, hi = BINARIES[b]["vram"], LIMITS[b]
    pairs = []
    users = ("main", "field", "battle", "viewer") if b == "main" else (b,)
    for user in users:
        pairs += reloc_refs(refs[user])
    syms = {s["name"]: s["value"] for s in elf32.Elf(os.path.join(ROOT, refs[b])).symbols() if s["name"]}
    pairs += provide_refs(ld_script.c_syms_path(b), syms)
    # accesses through pointers (tools/ptr_scan.py): from the symbol to the
    # first and the last byte reached; +1 so that a last byte at the end of a
    # unit doesn't count as an end pointer to the next one
    for base, a0, a1, user, site in ptr_scan.scan(refs, users):
        pairs.append((base, min(base, a0), "ptr"))
        pairs.append((base, max(base, a1 - 1) + 1, "ptr"))
    ranges = sorted({(min(s, t), max(s, t)) for s, t, n in pairs
                     if lo <= s < hi and lo <= t < hi and s != t and abs(s - t) < BIASED})
    with open(cache, "w") as f:
        json.dump({"stamp": stamp, "version": TOGETHER_VERSION, "ranges": ranges}, f)
    return ranges


class Cluster:
    """Units that move together (see together())."""

    def __init__(self, units):
        self.units = units
        self.addr = units[0].addr
        self.size = max(u.end for u in units) - self.addr
        self.nobits = all(u.nobits for u in units)
        self.pinned = any(u.pinned for u in units)
        self.outsec = units[0].outsec
        self.new = None

    @property
    def end(self):
        return self.addr + self.size

    def __repr__(self):
        return f"<{len(self.units)} units from {self.units[0]!r}>"


def make_clusters(b, us):
    """Group the sized units of b's own output sections into clusters; a
    cluster with a pinned unit is pinned as a whole."""
    sized = sorted((u for u in us if u.size and u.outsec != ".lay_subprog1"), key=lambda u: u.addr)
    starts = [u.addr for u in sized]
    glue = [False] * len(sized)  # glue[i]: unit i and i+1 move together
    for s, t in together(b):
        i = max(bisect.bisect_right(starts, s) - 1, 0)
        j = max(bisect.bisect_right(starts, t) - 1, 0)
        if t == sized[j].addr and j > i:
            j -= 1  # an end pointer: the unit before t
        for k in range(i, j):
            glue[k] = True
    out = []
    cur = []
    for k, u in enumerate(sized):
        cur.append(u)
        if not glue[k]:
            out.append(Cluster(cur))
            cur = []
    if cur:
        out.append(Cluster(cur))
    for c in out:
        if c.pinned:
            first = next(u for u in c.units if u.pinned)
            for u in c.units:
                if not u.pinned:
                    u.pinned = True
                    u.why = f"moves with {first.obj}({first.sec})"
    return out


# ------------------------------------------------------------------ placement
class Space:
    """Occupied address ranges, sorted and merged."""

    def __init__(self):
        self.lo, self.hi = [], []

    def add(self, a, b):
        i = bisect.bisect_left(self.lo, a)
        self.lo.insert(i, a)
        self.hi.insert(i, b)

    def fit(self, lb, size, residue, limit=None):
        """Lowest a >= lb with a % RESIDUE == residue and [a, a+size) free."""
        a = lb + ((residue - lb) % RESIDUE)
        i = max(bisect.bisect_right(self.lo, a) - 1, 0)
        while True:
            # skip ranges that end at or before a
            while i < len(self.lo) and self.hi[i] <= a:
                i += 1
            if i < len(self.lo) and self.lo[i] < a + size:
                a = self.hi[i] + ((residue - self.hi[i]) % RESIDUE)
                continue
            if limit is not None and a + size > limit:
                return None
            return a


def point_addrs(b, us):
    """Baseline address of each padding point."""
    texts = [u for u in us if u.sec.startswith(".text") and u.size]
    secs = BINARIES[b]["sections"]
    return {"text": texts[1].addr, "data": secs["data"], "rodata": secs["rodata"],
            "bss": secs.get("scommon", secs["bss"])}


def is_code(b, c):
    """A cluster of EE code only: functions don't run into the next unit
    (checked: every .text unit ends in a jump and its delay slot, or a
    syscall that doesn't return), so code may fill gaps out of order. Data
    (.vutext included: VU microcode sent by DMA) keeps its order, so that
    objects stay next to the ones they were next to wherever no pinned unit
    is in the way: splat's object boundaries are guesses, and code may reach
    a neighbour through a pointer or an index that no relocation shows."""
    code_end = BINARIES[b]["vram"] + BINARIES[b]["text_end"]
    return all(u.sec.startswith(".text") and u.end <= code_end for u in c.units)


def place(b, us, pads):
    """Give every unit its new address (u.new). pads: {point address: size}."""
    space = Space()
    own = make_clusters(b, us)
    for u in own:
        if u.pinned:
            u.new = u.addr
            space.add(u.addr, u.end)
    fixed = [u for u in us if u.outsec == ".lay_subprog1"]
    for u in fixed:
        u.new = u.addr
    first_pad = min(pads) if pads else None
    pending = sorted(pads.items())
    loads = [u for u in own if not u.nobits]
    zeros = [u for u in own if u.nobits]
    nobits_start = min(u.addr for u in zeros if u.size)
    pinned_zero = sorted(u.addr for u in zeros if u.pinned)
    pinned_zero_end = max([u.end for u in zeros if u.pinned] or [nobits_start])

    def flow(group, lb_of, limit_of=lambda u: None, overflow=None, order=None):
        """Place group's free units; overflow(u) gives the lower bound to use
        when a unit doesn't fit below its limit (None: fail). Code units fill
        gaps; every other unit goes above all units placed before it in the
        group (order[0], shared when given), so data keeps its order."""
        nonlocal pending
        order = [0] if order is None else order
        for u in group:
            if u.pinned:
                continue
            while pending and pending[0][0] <= u.addr and u.size:
                x, size = pending.pop(0)
                a = space.fit(max(x, order[0]), size, x % RESIDUE)
                space.add(a, a + size)  # the padding itself is reserved
                order[0] = max(order[0], a + size)
            if first_pad is None or u.addr < first_pad:
                u.new = u.addr  # before any padding nothing moves
                if u.size:
                    space.add(u.addr, u.end)
                    order[0] = max(order[0], u.end)
                continue
            if not u.size:
                continue  # placed with the unit before it (see attach_empty)
            lb = lb_of(u)
            if not is_code(b, u):
                lb = max(lb, order[0])
            a = space.fit(lb, u.size, u.addr % RESIDUE, limit_of(u))
            if a is None:
                if overflow is None:
                    raise SystemExit(f"{b}: no room for {u} below {limit_of(u):08X}")
                a = space.fit(max(lb, overflow(u)), u.size, u.addr % RESIDUE)
            u.new = a
            space.add(a, a + u.size)
            order[0] = max(order[0], a + u.size)

    lo = first_pad if first_pad is not None else 0
    if b == "main":
        # initialized units: below the first pinned .bss object (not .scommon)
        bss_start = BINARIES[b]["sections"]["bss"]
        first_pinned_bss = min([a for a in pinned_zero if a >= bss_start] or [LIMITS[b]])
        flow(loads, lambda u: lo, lambda u: first_pinned_bss)
        load_end = max(u.new + u.size for u in loads if u.size)
        fbss = next(c for c in zeros if any(u.sec == f".bss.{MAIN_FBSS}" for u in c.units))
        before = [u for u in zeros if u.addr < fbss.addr]
        after = [u for u in zeros if u.addr >= fbss.addr]
        flow(before, lambda u: max(lo, nobits_start))
        state = {"fbss": None}

        def lb_after(u):
            if state["fbss"] is None:
                return max(lo, load_end)
            return state["fbss"]
        order = [0]
        for u in after:
            flow([u], lb_after, order=order)
            if u is fbss:
                state["fbss"] = u.new
        if any(u.new < state["fbss"] and not u.pinned for u in after if u.size):
            raise SystemExit("main: a .bss object went below crt0's clear start")
    else:
        flow(loads, lambda u: lo, lambda u: nobits_start, lambda u: pinned_zero_end)
        flow(zeros, lambda u: max(lo, nobits_start))
    for c in own:
        for u in c.units:
            u.new = c.new + (u.addr - c.addr)
    attach_empty(us)
    return us


def attach_empty(us):
    """Zero-sized units (labels only) move with the sized unit before them in
    their output section."""
    prev = {}
    for u in us:
        if u.size:
            prev[u.outsec] = u
        elif u.new is None:
            p = prev.get(u.outsec)
            u.new = p.new + (u.addr - p.addr) if p else u.addr


def check_layout(b, us):
    sized = sorted((u.new, u.new + u.size, u) for u in us if u.size)
    for (a0, a1, u0), (b0, b1, u1) in zip(sized, sized[1:]):
        if b0 < a1:
            raise SystemExit(f"{b}: {u0} and {u1} overlap at {b0:08X}")
    end = max(a1 for a0, a1, u in sized if u.outsec != ".lay_subprog1")
    if end > LIMITS[b]:
        raise SystemExit(f"{b}: ends at {end:08X}, past {LIMITS[b]:08X} (too much padding)")
    for u in us:
        if u.pinned and u.new != u.addr:
            raise SystemExit(f"{b}: pinned {u} moved")
    return end


# ------------------------------------------------------------------ script
def write_script(b, us, path):
    """A linker script that puts every unit at its new address."""
    vram = BINARIES[b]["vram"]
    src = open(os.path.join(ROOT, ld_script.source_script(b))).read()
    gp = re.search(r"^\s*(_gp = 0x[0-9A-Fa-f]+;)", src, re.M)
    lines = ["/* Pinned link (tools/pin_link.py): every unit at an explicit address */",
             "SECTIONS", "{"]
    if gp:
        lines.append(f"    {gp.group(1)}")
    # zero-sized input sections hold no symbols (checked when this was
    # written), so they are left out
    own = sorted((u for u in us if u.outsec != ".lay_subprog1" and u.size), key=lambda u: u.new)
    if b == "main":
        load_end = max(u.new + u.size for u in own if not u.nobits and u.size)
        if any(u.new != u.addr for u in own):
            # everything below crt0's clear start is in the file (as zeros):
            # the original leaves .scommon's first 0x70 bytes neither loaded
            # nor cleared, and whatever the loader leaves there would now be
            # other objects
            fbss = next(u for u in own if u.sec == f".bss.{MAIN_FBSS}")
            load_end = max(load_end, fbss.new)
        groups = [(".main", vram, "", [u for u in own if u.new < load_end or (not u.nobits)]),
                  (".main_bss", None, " (NOLOAD)", [u for u in own if u.new >= load_end and u.nobits])]
    else:
        groups = [(OUTSEC[b], vram, "", own)]
    for name, start, kind, group in groups:
        group = sorted(group, key=lambda u: u.new)
        if start is None:
            start = group[0].new
        seg = name.lstrip(".")
        lines.append(f"    {seg}_VRAM = ADDR({name});")
        lines.append(f"    {name} {start:#x}{kind} : AT({start:#x})")
        lines.append("    {")
        lines.append("        FILL(0x00000000);")
        for u in group:
            lines.append(f"        . = {u.new - start:#x}; {u.obj}({u.sec});"
                         + (f" /* pinned: {u.why} */" if u.pinned else ""))
        lines.append("    }")
    fixed = [u for u in us if u.outsec == ".lay_subprog1" and u.size]
    if fixed:
        start = min(u.new for u in fixed)
        lines.append("    lay_subprog1_VRAM = ADDR(.lay_subprog1);")
        lines.append(f"    .lay_subprog1 {start:#x} : AT({start:#x})")
        lines.append("    {")
        for u in sorted(fixed, key=lambda u: u.new):
            lines.append(f"        . = {u.new - start:#x}; {u.obj}({u.sec});")
        lines.append("    }")
    lines += ["    /DISCARD/ :", "    {", "        *(*);", "    }", "}"]
    for sym, const in ld_script.VRAM_ASSERTS[b]:
        lines.append(f'ASSERT({sym} == {const}, "{sym} differs from {const} in config/memory_map.ld");')
    os.makedirs(os.path.dirname(os.path.join(ROOT, path)), exist_ok=True)
    with open(os.path.join(ROOT, path), "w") as f:
        f.write("\n".join(lines) + "\n")


def link(b, script, out, csyms, relocs=True):
    cmd = [LD, "-EL", "--no-check-sections"] + (["-q"] if relocs else []) + ["-Map", out[:-4] + ".map", "-o", out]
    run(cmd + ld_script.link_scripts(b, script, csyms))


def verify_link(b, us, map_path):
    """The linked addresses must be the planned ones."""
    got = {(u.obj, u.sec): u.addr for u in map_units(map_path) if u.size}
    bad = [u for u in us if u.size and got.get(u.key()) != u.new]
    if bad:
        u = bad[0]
        raise SystemExit(f"{b}: {len(bad)} units not where planned, e.g. {u} planned {u.new:08X} "
                         f"linked {got.get(u.key(), 0):08X}")


def pinned_link(b, pads, out_prefix=None, main_elf=None, relocs=True):
    """Place and link binary b with pads {point name or 0xADDR: size}. Returns
    (units, elf path, end address)."""
    if b not in LIMITS:
        raise SystemExit(f"pinned links are for main and battle, not {b}")
    out_prefix = out_prefix or f"{PIN}/{b}"
    us = units(b)
    mark_pins(b, us)
    pts = point_addrs(b, us)
    padd = {}
    for p, size in pads.items():
        if size % RESIDUE:
            raise SystemExit(f"padding {size:#x} at {p} isn't a multiple of {RESIDUE:#x}")
        a = pts[p] if p in pts else int(p, 16)
        padd[a] = padd.get(a, 0) + size
    place(b, us, padd)
    end = check_layout(b, us)
    script = out_prefix + ".ld"
    write_script(b, us, script)
    csyms = ld_script.c_syms_path(b)
    if b != "main":
        csyms = out_prefix + ".c_syms.ld"
        run(["python3"] + ld_script.c_syms_args(b, csyms, main_elf))
    elf = out_prefix + ".elf"
    link(b, script, elf, csyms, relocs)
    verify_link(b, us, out_prefix + ".map")
    with open(os.path.join(ROOT, out_prefix + ".layout"), "w") as f:
        f.write(f"# {b} pinned layout, pads {', '.join(f'{p}={s:#x}' for p, s in pads.items()) or 'none'}; "
                f"end {end:08X}\n# baseline new delta pinned unit\n")
        for u in sorted(us, key=lambda u: u.addr):
            if u.size:
                f.write(f"{u.addr:08X} {u.new:08X} {u.new - u.addr:+#x} {'P' if u.pinned else '-'} "
                        f"{u.obj}({u.sec})\n")
    return us, elf, end


def parse_pads(items):
    pads = {}
    for item in items or []:
        p, _, size = item.partition("=")
        pads[p] = pads.get(p, 0) + int(size, 0)
    return pads


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("binary", choices=sorted(LIMITS))
    ap.add_argument("--pad", nargs="*", default=[])
    ap.add_argument("--main-elf")
    ap.add_argument("--out")
    args = ap.parse_args()
    os.chdir(ROOT)
    us, elf, end = pinned_link(args.binary, parse_pads(args.pad), args.out, args.main_elf)
    moved = sum(1 for u in us if u.size and u.new != u.addr)
    pinned = sum(1 for u in us if u.pinned)
    orig_end = max(u.end for u in us if u.size and u.outsec != ".lay_subprog1")
    print(f"{args.binary}: {elf}: {pinned} pinned units, {moved} moved, end {end:08X} "
          f"(grew {end - orig_end:#x}, limit {LIMITS[args.binary]:08X})")


if __name__ == "__main__":
    main()
