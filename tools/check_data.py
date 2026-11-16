#!/usr/bin/env python3
"""Check one data-only C file (src/<binary>/data/<ADDR>.c, plan step 6b)
against the original binary, without a split or make.

    tools/docker.sh python3 tools/check_data.py path/to/6E0198.c [-v] [--keep]

The file claims its range with `/* data: 0xSTART-0xEND */` and/or
`/* rodata: 0xSTART-0xEND */` (see tools/data_asm.py). The check compiles it
the way the build does (tools/compile_c.py) and, for each claimed section:

  - size and alignment: the section must fit the range, its alignment must
    divide the start address, and the bytes after it up to the end must be
    zeros that the next piece's alignment recreates;
  - bytes: every byte equals the original, with each relocation resolved
    (symbol address from config/symbol_addrs*.txt, config/memory_map.ld or
    the D_/func_ name, plus the addend in the word);
  - relocations: every word the asm piece wrote as a symbol must be a
    relocation in C (a pointer written as a number doesn't shift), and no
    other word may be one (unless --allow-extra);
  - symbols: every global the file defines must be at the address its
    symbol-file entry or D_ name says, and every asm label in the range must
    be defined there or fall inside an object (a field, which
    tools/c_syms.py defines relative to it);
  - nothing else: no .text, no section the file doesn't claim (the linker
    script discards sections it doesn't list).

The asm reference comes from the current asm/ pieces that cover the range.
Each run caches it in build/check_data/ref/, so the file can still be
checked after the range has been taken out of asm (after integration).

Safe to run in parallel: it compiles into build/check_data/<binary>_<file>_<pid>/
and removes it afterwards (unless --keep). Exit status 0 only if all is OK.
"""
import argparse
import json
import os
import shutil
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elf32  # noqa: E402
from compile_c import compile_c  # noqa: E402
from data_asm import (ROOT, Symbols, asm_pieces, binary_for_data, defined_objects, markers,  # noqa: E402
                      natural_align, parse_expr, rom, section_ranges)
from binaries import BINARIES  # noqa: E402

OWN = (".data", ".rodata")
EMPTY_OK = {".bss", ".reginfo", ".mdebug.eabi64", ".mdebug.eabi32", ".comment", ".pdr", ".gcc_compiled_long32",
            ".gcc_compiled_long64", ".note.GNU-stack", ".mdebug", ".gnu.attributes", ".MIPS.abiflags"}


def reference(b, start, end, sec):
    """{'symbolic': {addr: expr}, 'labels': {addr: [names]}, 'source': [...]} from the
    asm pieces covering [start, end), cached in build/check_data/ref/."""
    cache = os.path.join(ROOT, "build/check_data/ref", b, f"{start:08X}-{end:08X}.json")
    pieces = asm_pieces(b, start, end)
    covered = start
    for p in pieces:
        if p.start > covered:
            break
        covered = max(covered, p.end)
    if covered >= end and all(p.section == sec for p in pieces):
        ref = {"symbolic": {}, "labels": {}, "source": [os.path.relpath(p.path, ROOT) for p in pieces]}
        for p in pieces:
            for v, op in p.symbolic_words().items():
                if start <= v < end:
                    ref["symbolic"][str(v)] = op
            for v, names in p.labels.items():
                if start <= v < end:
                    ref["labels"].setdefault(str(v), []).extend(names)
        os.makedirs(os.path.dirname(cache), exist_ok=True)
        tmp = f"{cache}.{os.getpid()}"
        with open(tmp, "w") as f:
            json.dump(ref, f, indent=0, sort_keys=True)
        os.replace(tmp, cache)
        return ref, None
    if os.path.exists(cache):
        return json.load(open(cache)), "cached (the range is no longer asm)"
    if pieces and covered < end:
        return None, f"asm covers only up to 0x{covered:08X} and no cached reference: relocations and labels not checked"
    if pieces and any(p.section != sec for p in pieces):
        return None, f"the range is {pieces[0].section} in the asm, not {sec}"
    return None, "the range is not asm and no cached reference: relocations and labels not checked"


def main():
    ap = argparse.ArgumentParser(description="Check one data-only C file against the original.")
    ap.add_argument("c_file")
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--keep", action="store_true", help="keep the build folder")
    ap.add_argument("--allow-extra", action="store_true",
                    help="accept relocations where the asm has a plain number (a pointer splat missed)")
    a = ap.parse_args()
    os.chdir(ROOT)
    text = open(a.c_file).read()
    try:
        claims = markers(text)
    except ValueError as e:
        sys.exit(f"{a.c_file}: {e}")
    if not claims:
        sys.exit(f"{a.c_file}: no /* data: 0xSTART-0xEND */ or /* rodata: 0xSTART-0xEND */ marker")
    bins = set()
    for sec, (s, e) in claims.items():
        b, real = binary_for_data(s)
        lo, hi = section_ranges(b)[real]
        if real != sec:
            sys.exit(f"{a.c_file}: 0x{s:08X} is in {b}'s {real}, but the marker says {sec}")
        if not lo <= s < e <= hi:
            sys.exit(f"{a.c_file}: {sec} range 0x{s:08X}-0x{e:08X} leaves {b}'s {sec} (0x{lo:08X}-0x{hi:08X})")
        bins.add(b)
    if len(bins) != 1:
        sys.exit(f"{a.c_file}: claims ranges in several binaries")
    b = bins.pop()
    image = rom(b)
    vram = BINARIES[b]["vram"]
    syms = Symbols(b)
    stem = os.path.splitext(os.path.basename(a.c_file))[0]
    work = os.path.join(ROOT, "build/check_data", f"{b}_{stem}_{os.getpid()}")
    os.makedirs(work, exist_ok=True)
    errors, notes = [], []

    def err(msg):
        errors.append(msg)

    try:
        obj = os.path.join(work, f"{stem}.o")
        ok, msg = compile_c(a.c_file, obj, os.path.join(work, f"{stem}.s"))
        if not ok:
            sys.exit(msg)
        elf = elf32.Elf(obj)
        secs = {s["name"]: s for s in elf.sections}
        for s in elf.sections:
            if s["flags"] & elf32.SHF_ALLOC and s["size"] and s["name"] not in claims:
                err(f"{s['name']}: {s['size']:#x} bytes, but the file claims no {s['name']} range "
                    f"(the link would discard it). String literals in a .data file go to .rodata: "
                    f"refer to the original string by name instead")
        base = {sec: claims[sec][0] for sec in claims}
        all_syms = elf.symbols()

        def sym_addr(s):
            """Linked address of an object-file symbol, or None."""
            if s["shndx"] not in (elf32.SHN_UNDEF, elf32.SHN_ABS, elf32.SHN_COMMON):
                sname = elf.sections[s["shndx"]]["name"]
                if sname in base:
                    return base[sname] + s["value"]
                return None
            return syms.lookup(s["name"])

        defined_here = {}
        for s in all_syms:
            if s["name"] and s["shndx"] not in (elf32.SHN_UNDEF, elf32.SHN_ABS, elf32.SHN_COMMON) \
                    and s["type"] in (0, 1) and s["bind"] == 1:
                sname = elf.sections[s["shndx"]]["name"]
                if sname in claims:
                    defined_here[s["name"]] = (sname, base[sname] + s["value"], s["size"])

        for sec, (start, end) in claims.items():
            s = secs.get(sec)
            if s is None or not s["size"]:
                err(f"{sec}: the file claims 0x{start:08X}-0x{end:08X} but emits no {sec}")
                continue
            data = elf.data(s)
            size, align = s["size"], max(s["align"], 1)
            what = f"{sec} 0x{start:08X}-0x{end:08X}"
            if start % align:
                err(f"{what}: the section is {align}-aligned (its first object needs it), "
                    f"but starts at 0x{start:08X}; start the range at an address aligned to {align}")
            if size > end - start:
                err(f"{what}: {size:#x} bytes emitted, the range is {end - start:#x}")
            else:
                pad_to = -(-(start + size) // natural_align(end)) * natural_align(end)
                if size < end - start and pad_to != end:
                    err(f"{what}: {size:#x} bytes emitted, the range is {end - start:#x}, and the "
                        f"next piece's alignment ({natural_align(end)}) only pads to 0x{pad_to:08X}: "
                        f"give the last object an explicit size")
            # Relocations, then the bytes with them applied
            relocs = {}
            for off, typ, sym, add, tsec in elf.relocs(sec):
                if typ != elf32.R_MIPS_32:
                    err(f"{sec}+{off:#x}: relocation type {elf32.RELOC_NAMES.get(typ, typ)} (only R_MIPS_32 expected)")
                    continue
                relocs[off] = sym
            got = bytearray(data)
            for off, sym in sorted(relocs.items()):
                t = sym_addr(sym)
                if t is None:
                    err(f"{start + off:08X}: relocation against {sym['name'] or '(section)'} whose "
                        f"address isn't known (a name not in the symbol files, or a section the file doesn't claim)")
                    continue
                w = struct.unpack_from("<I", got, off)[0]
                struct.pack_into("<I", got, off, (t + w) & 0xFFFFFFFF)
            want = image[start - vram:end - vram]
            n = min(len(got), len(want))
            bad = [i for i in range(n) if got[i] != want[i]]
            if len(got) < len(want) and any(want[len(got):]):
                err(f"{what}: the original has non-zero bytes after the compiled {size:#x} "
                    f"(at 0x{start + len(got) + next(i for i, c in enumerate(want[len(got):]) if c):08X})")
            if bad:
                err(f"{what}: {len(bad)} bytes differ")
                shown = set()
                for i in bad:
                    w = i & ~3
                    if w in shown:
                        continue
                    shown.add(w)
                    if len(shown) > 12:
                        errors.append("    ...")
                        break
                    gw = got[w:w + 4].hex(" ")
                    ow = want[w:w + 4].hex(" ")
                    tag = f" (reloc {relocs[w]['name'] or 'section'})" if w in relocs else ""
                    errors.append(f"    {start + w:08X}: compiled {gw:<11} original {ow}{tag}")

            # Against the asm reference
            ref, why = reference(b, start, end, sec)
            if ref is None:
                notes.append(f"{what}: {why}")
                continue
            if why:
                notes.append(f"{what}: asm reference {why}")
            exp = {int(k): v for k, v in ref["symbolic"].items()}
            for v, op in sorted(exp.items()):
                off = v - start
                if off not in relocs:
                    err(f"{v:08X}: the asm has `.word {op}` but the C word is a plain number "
                        f"(write it as the symbol's address)")
                    continue
                name, add = parse_expr(op)
                cs = relocs[off]
                cname = cs["name"]
                if cname != name and a.verbose:
                    t = sym_addr(cs)
                    notes.append(f"{v:08X}: asm `{op}`, C {cname or elf.sections[cs['shndx']]['name']}"
                                 f"{' (0x%08X)' % t if t is not None else ''} + addend: same address")
            for off, sym in sorted(relocs.items()):
                if start + off not in exp:
                    msg = (f"{start + off:08X}: C has a relocation ({sym['name'] or 'section'}) where the asm "
                           f"has a plain number")
                    if a.allow_extra:
                        notes.append(msg + " (allowed)")
                    else:
                        err(msg + " (a pointer splat missed? then pass --allow-extra and say so)")
            labels = {int(k): v for k, v in ref["labels"].items()}
            objs = sorted((addr, sz, n) for n, (sname, addr, sz) in defined_here.items() if sname == sec)
            for addr, sz, n in objs:
                if addr in labels and n not in labels[addr]:
                    err(f"{addr:08X}: object {n}, but the asm label there is "
                        f"{' / '.join(labels[addr])}: use that name")
            for v, names in sorted(labels.items()):
                for n in names:
                    if n in defined_here:
                        continue
                    inside = [(addr, sz, on) for addr, sz, on in objs if addr <= v < addr + max(sz, 1)]
                    if inside:
                        addr, sz, on = inside[-1]
                        if a.verbose:
                            notes.append(f"{v:08X}: {n} is a field: {on} + 0x{v - addr:X} (c_syms.py defines it)")
                    elif sec == ".rodata":
                        # a string literal or constant: c_syms.py defines the
                        # name relative to the file's __rodata_ marker
                        if a.verbose:
                            notes.append(f"{v:08X}: {n} is in the file's anonymous .rodata (a literal); "
                                         f"c_syms.py defines it relative to __rodata_{b}_data_{stem}")
                    else:
                        err(f"{v:08X}: asm label {n} is neither defined nor inside an object of the file")

        # Every global the file defines is at its address
        for n, (sname, addr, sz) in sorted(defined_here.items(), key=lambda x: x[1][1]):
            want_addr = syms.lookup(n)
            if want_addr is None:
                err(f"{n}: not in the symbol files and not a D_<address> name (give it one, "
                    f"or make it static)")
            elif want_addr != addr:
                err(f"{n}: defined at 0x{addr:08X}, but its address is 0x{want_addr:08X}")
        # tools/c_syms.py finds the objects by reading the source; it must see
        # exactly what the compiler defined (fields are placed relative to them)
        seen = set(defined_objects(text))
        for n in sorted(set(defined_here) - seen):
            err(f"{n}: tools/c_syms.py can't find its definition in the source (write it as "
                f"`<type> {n}[...] = ...;` at the top level)")
        for n in sorted(seen - set(defined_here)):
            err(f"{n}: looks like a global definition in the source, but the object defines no such symbol")
        # ... and isn't defined by another piece too
        others = set()
        for p in asm_pieces(b):
            for v, names in p.labels.items():
                if not any(s <= v < e for s, e in claims.values()):
                    others.update(names)
        for n in sorted(set(defined_here) & others):
            err(f"{n}: also a label in another asm piece (defined twice at link time)")
    finally:
        if not a.keep:
            shutil.rmtree(work, ignore_errors=True)

    for n in notes:
        print(f"note: {n}")
    if errors:
        for e in errors:
            print(e if e.startswith("    ") else f"error: {e}")
        print(f"{a.c_file}: FAILED ({sum(1 for e in errors if not e.startswith('    '))} errors)")
        sys.exit(1)
    print(f"{a.c_file}: OK ({', '.join(f'{s} 0x{x:08X}-0x{y:08X}' for s, (x, y) in claims.items())}; "
          f"{len(defined_here)} objects)")


if __name__ == "__main__":
    main()
