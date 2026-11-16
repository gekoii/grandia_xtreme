#!/usr/bin/env python3
"""Write a binary's .scommon/.bss objects as C definitions (plan step 6a).

Usage: gen_bss.py <binary> [--write] [--force]

Reads the labels splat put in the binary's assembly .bss pieces
(asm/<binary>/data/<binary>/*.bss.s, so run it while the splat config still
has `bss` subsegments) and prints, or with --write writes, one C file per
section (src/<binary>/bss_<ADDR>.c, named after its first object): every
object in address order, as a plain array sized from the layout (u8 x[size])
or a u8/u16/u32 scalar where the size and address allow it.

How the files reproduce the layout (see docs/shifting.md):
  - gcc 2.9 for this target has no way to emit an uninitialized global into
    .bss in definition order: as a tentative definition it becomes COMMON
    (allocated by ld in hash order), and with -fno-common it goes to .data.
    The objects are therefore given __attribute__((section(".bss"))), which
    emits them in .bss in definition order (the files are also compiled with
    -fno-common, so a definition without it can't become COMMON).
  - gcc aligns every array to 8 (DATA_ALIGNMENT, BITS_PER_WORD = 64), and a
    scalar to its size. Arrays get an explicit alignment (16 where the
    address allows it, else 8, never more than the file's start has); the
    first object of a .bss section gets 128, which is how the original
    linker script aligned .bss.
  - A label that can't start an object of its own (an array at an address
    that isn't 8-aligned, a field at an odd address) is a field of the
    object before it: it becomes part of that object, and tools/c_syms.py
    defines its name relative to it. Zero bytes after a scalar that the next
    object's alignment recreates are left out of the scalar.

The files are source after that: types from phase 4 can replace the arrays,
as long as sizes and alignment stay the same (the build checks every
address).
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from binaries import BINARIES  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

# Per binary, each section as one C file: (first object, end, alignment of
# the first object, alignment of what follows the end). The end of .scommon is
# where .bss starts (FIELD, BATTLE) or the ELF's .scommon end (main); the end
# of .bss is the end of the binary (an overlay's file length).
SECTIONS = {
    "main": [(0x004A9410, 0x004A962C, None, 128, ".scommon"),
             (0x004A9680, 0x005A6B20, 128, 1, ".bss")],
    "field": [(0x006762B8, 0x00676480, None, 128, ".scommon"),
              (0x00676480, 0x0067F646, 128, 1, ".bss")],
    "battle": [(0x006E3E80, 0x006E3F00, None, 128, ".scommon"),
               (0x006E3F00, 0x006F9350, 128, 1, ".bss")],
    "viewer": [(0x00703500, 0x0070DF44, 128, 1, ".bss")],
}

DLABEL_RE = re.compile(r"^\s*dlabel\s+(\w+)")
VRAM_RE = re.compile(r"/\*\s+(?:[0-9A-F]+\s+)?([0-9A-F]{8})\b")


def labels(b):
    """(addr, name) of every label in the binary's asm .bss pieces, except
    the fixed addresses of config/memory_map.ld (VIEWER's .bss covers the
    battle character-program slots, which splat labels by name)."""
    from c_syms import fixed_names
    fixed = fixed_names()
    out = []
    for path in sorted(glob.glob(os.path.join(ROOT, f"asm/{b}/data/{b}/*.bss.s"))):
        lines = open(path).read().splitlines()
        for i, line in enumerate(lines):
            m = DLABEL_RE.match(line)
            if not m:
                continue
            for nxt in lines[i + 1:i + 4]:
                c = VRAM_RE.search(nxt)
                if c:
                    if m.group(1) not in fixed:
                        out.append((int(c.group(1), 16), m.group(1)))
                    break
    return sorted(set(out))


def natural(a):
    n = 1
    while a % (n * 2) == 0 and n < 0x1000:
        n *= 2
    return n


def align_up(x, n):
    return (x + n - 1) // n * n


SCALAR = {1: "u8", 2: "u16", 4: "u32"}


def classify(a, s, next_align, first_align, cap):
    """(type, declared size, alignment, explicit alignment or None) of an
    object at a that runs s bytes to the next object, or None if it can't be
    one object there."""
    if first_align:
        # The section's first object: aligned like the section
        if s in SCALAR and a % s == 0:
            return SCALAR[s], s, first_align, first_align
        return "u8[]", s, first_align, first_align
    if s in SCALAR and a % s == 0:
        return SCALAR[s], s, s, None
    if a % 8 == 0:
        al = min(natural(a), 16, cap)
        return "u8[]", s, al, al
    # A scalar followed by padding the next object's alignment recreates
    for d in (4, 2, 1):
        if a % d == 0 and d < s and align_up(a + d, next_align) == a + s:
            return SCALAR[d], d, d, None
    return None


def plan(b):
    labs = labels(b)
    files = []
    for start, end, first_align, after_align, what in SECTIONS[b]:
        ls = [(a, n) for a, n in labs if start <= a < end]
        if not ls or ls[0][0] != start:
            raise SystemExit(f"{b}: no label at the start of {what} 0x{start:08X}")
        objs = []  # (addr, name, type, size, explicit align, [(off, interior name)])
        # Offsets in the file's .bss are aligned relative to its start, so no
        # object may need more alignment than the start has (FIELD's
        # .scommon starts only 8-aligned)
        cap = first_align or natural(start)
        nxt_addr, nxt_align = end, after_align
        interior = []
        for i in range(len(ls) - 1, -1, -1):
            a, n = ls[i]
            r = classify(a, nxt_addr - a, nxt_align, first_align if i == 0 else None, cap)
            if r is None:
                interior.append((a, n))
                continue
            typ, size, al, explicit = r
            inner = sorted((x - a, m) for x, m in interior if a < x < nxt_addr)
            interior = [(x, m) for x, m in interior if not a < x < nxt_addr]
            objs.append((a, n, typ, size, explicit, inner))
            nxt_addr, nxt_align = a, al
        if interior:
            raise SystemExit(f"{b}: labels left before the first object: {interior}")
        objs.reverse()
        files.append((start, end, what, objs))
    # Labels outside every section (a piece-start label in an alignment gap)
    covered = [(s, e) for s, e, *_ in SECTIONS[b]]
    dropped = [(a, n) for a, n in labs if not any(s <= a < e for s, e in covered)]
    return files, dropped


HEADER = """\
/* {title}: every object in address order (plan step 6a, docs/shifting.md).
 * Generated by tools/gen_bss.py; types are plain arrays sized from the
 * layout, or u8/u16/u32 where the size and address make that obvious.
 * Names in comments are fields inside an object (labels at an address no C
 * object of their own could start at); tools/c_syms.py defines them
 * relative to it. Don't include the project headers here: the objects'
 * types there may differ, which C linkage allows.
 */
/* cflags: -fno-common */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

/* gcc 2.9 puts an uninitialized -fno-common global in .data on this target */
#define BSS __attribute__((section(".bss")))
#define ALIGN(n) __attribute__((aligned(n)))

"""

TITLES = {"main": "the main executable", "field": "FIELD", "battle": "BATTLE", "viewer": "VIEWER"}


def render(b, start, end, what, objs):
    title = f"{what} of {TITLES[b]}, 0x{start:08X}-0x{end:08X}"
    out = [HEADER.format(title=title)]
    for a, n, typ, size, explicit, inner in objs:
        if typ == "u8[]":
            decl = f"u8 {n}[0x{size:X}]"
        else:
            decl = f"{typ} {n}"
        attrs = " BSS" + (f" ALIGN({explicit})" if explicit else "")
        line = f"{decl}{attrs}; /* {a:08X}"
        if inner:
            line += "; " + ", ".join(f"+0x{off:X} {m}" for off, m in inner)
        out.append(line + " */\n")
    return "".join(out)


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if len(args) != 1 or args[0] not in SECTIONS:
        raise SystemExit(__doc__)
    b = args[0]
    os.chdir(ROOT)
    files, dropped = plan(b)
    total = sum(len(o) for *_, o in files)
    inner = sum(len(x[5]) for *_, o in files for x in o)
    for start, end, what, objs in files:
        path = os.path.join(ROOT, f"src/{b}/bss_{start:06X}.c")
        text = render(b, start, end, what, objs)
        if "--write" in sys.argv:
            if os.path.exists(path) and "--force" not in sys.argv:
                raise SystemExit(f"{path} exists (--force to overwrite)")
            open(path, "w").write(text)
            print(f"wrote {os.path.relpath(path, ROOT)}: {len(objs)} objects")
        else:
            sys.stdout.write(f"==== {os.path.relpath(path, ROOT)}\n{text}")
    print(f"{b}: {total} objects, {inner} labels inside another object, "
          f"{len(dropped)} labels outside the sections {[n for _, n in dropped]}", file=sys.stderr)


if __name__ == "__main__":
    main()
