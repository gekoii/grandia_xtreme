#!/usr/bin/env python3
"""Write bootable binaries for a chosen shift: the main executable as a PS2
ELF and the overlays as .BIN files, ready for tools/iso_patch.py.

Usage (in the build container, after a normal `make`):
  tools/docker.sh python3 tools/boot_build.py <name> [--main-pin PADS]
      [--battle-pin PADS] [--main-pad PADS] [--field-pad PADS]
      [--battle-pad PADS] [--viewer-pad PADS]
  tools/docker.sh python3 tools/boot_build.py control|shift_field|shift_all_pinned

  PADS    text=0x400,data=0x80,rodata=0x100,bss=0x200 (the points of
          tools/shift_test.py; multiples of 0x80)
  --*-pin a pinned link (tools/pin_link.py): main and BATTLE keep the
          addresses the disc's external programs use (config/abi_*.txt)
  --*-pad a plain shift (splat's linker script with padding). For main or
          BATTLE this breaks battles: their ABI moves.

The presets:
  control           the normal build, unchanged (checks the pipeline)
  shift_field       FIELD padded by 0x400 after its first .text object
  shift_all_pinned  main and BATTLE pinned with padding at text, data,
                    rodata and bss; FIELD and VIEWER padded at all points
  shift_main_pinned, shift_main_text, shift_main_bss, shift_battle_pinned,
  shift_battle_text, shift_battle_bss, shift_viewer, shift_field_all
                    one binary of shift_all_pinned at a time, at all its
                    points or only text or bss (docs/runtime_test.md,
                    "Bisecting"); shift_main_pinned_nopad, the pinned link
                    with no padding, gives control's files

Every relinked binary is checked first, exactly like tools/shift_test.py
(bytes, relocations, raw values, and for pinned links every ABI address in
place); a binary that moved wrongly stops the build. Binaries the case
doesn't change come from the normal build. Output in build/boot/<name>/:
SLUS_204.17, FIELD.BIN, BATTLE.BIN, VIEWER.BIN and report.txt.

The ELF: one PT_LOAD for the main executable from 0x00400000 (the
initialized bytes, then .scommon/.bss as memory size) and one for the
0x80-byte stub at 0x00700000, with p_paddr = p_vaddr, like the original
SLUS_204.17 (tools/elf2rom.py reads the same thing back). ld's own output
can't be used as is: splat's script gives the sections load addresses from
0 (rom offsets).
"""
import argparse
import hashlib
import os
import struct
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elf32  # noqa: E402
import shift_test  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
OUT = "build/boot"
FILES = {"main": "SLUS_204.17", "field": "FIELD.BIN", "battle": "BATTLE.BIN", "viewer": "VIEWER.BIN"}
SUBPROG = 0x00700000
ALL_POINTS = "text=0x400,data=0x80,rodata=0x100,bss=0x200"
PRESETS = {
    "control": {},
    "shift_field": {"field": ("pad", "text=0x400")},
    "shift_all_pinned": {
        "main": ("pin", ALL_POINTS),
        "field": ("pad", "text=0x400,data=0x80,rodata=0x100,bss=0x80"),  # FIELD has 0x9BA of room
        "battle": ("pin", ALL_POINTS),
        "viewer": ("pad", "text=0x400,rodata=0x100,bss=0x200"),  # no .data
    },
    # bisecting shift_all_pinned (docs/runtime_test.md, "Bisecting"): one
    # binary at a time, then one padding point at a time. The overlays are
    # relinked against a pinned main (they reference it) but not padded.
    "shift_main_pinned": {"main": ("pin", ALL_POINTS)},
    "shift_main_text": {"main": ("pin", "text=0x400")},
    "shift_main_bss": {"main": ("pin", "bss=0x200")},
    "shift_main_pinned_nopad": {"main": ("pin", "")},  # the pinned link's baseline: must equal control
    "shift_battle_pinned": {"battle": ("pin", ALL_POINTS)},
    "shift_battle_text": {"battle": ("pin", "text=0x400")},
    "shift_battle_bss": {"battle": ("pin", "bss=0x200")},
    "shift_viewer": {"viewer": ("pad", "text=0x400,rodata=0x100,bss=0x200")},
    "shift_field_all": {"field": ("pad", "text=0x400,data=0x80,rodata=0x100,bss=0x80")},
}


def parse_pads(text):
    out = []
    for item in filter(None, (text or "").split(",")):
        p, _, size = item.partition("=")
        out.append((p, int(size, 0)))
    return out


# ------------------------------------------------------------------ ELF
def write_ps2_elf(src_elf, out_path):
    """A PS2 executable from a linked main-executable ELF; returns its program
    headers [(vaddr, filesz, memsz)]."""
    e = elf32.Elf(os.path.join(ROOT, src_elf))
    secs = [s for s in e.alloc_sections()]
    segs = []
    for lo_ok in (lambda a: a < SUBPROG, lambda a: a >= SUBPROG):
        group = [s for s in secs if lo_ok(s["addr"])]
        if not group:
            continue
        start = min(s["addr"] for s in group)
        bits = [s for s in group if s["type"] != elf32.SHT_NOBITS]
        fend = max(s["addr"] + s["size"] for s in bits)
        mend = max(s["addr"] + s["size"] for s in group)
        img = bytearray(fend - start)
        for s in bits:
            img[s["addr"] - start:s["addr"] - start + s["size"]] = e.data(s)
        segs.append((start, bytes(img), mend - start))
    # layout: header, program headers, segment 1 at 0x80 (like the original),
    # then segment 2, then the section headers
    phoff, nph = 0x34, len(segs)
    off = 0x80
    places = []
    for start, img, memsz in segs:
        places.append(off)
        off = (off + len(img) + 15) & ~15
    shstr = b"\0.text\0.bss\0.lay_subprog1\0.shstrtab\0"
    shstr_off = off
    shoff = (shstr_off + len(shstr) + 3) & ~3
    shdrs = [bytes(40)]

    def sh(name, typ, flags, addr, offset, size, align):
        return struct.pack("<10I", shstr.index(name + b"\0"), typ, flags, addr, offset, size, 0, 0, align, 0)
    (s1, img1, mem1) = segs[0]
    shdrs.append(sh(b".text", 1, 7, s1, places[0], len(img1), 0x80))
    if mem1 > len(img1):
        shdrs.append(sh(b".bss", 8, 3, s1 + len(img1), places[0] + len(img1), mem1 - len(img1), 0x10))
    if len(segs) > 1:
        s2, img2, mem2 = segs[1]
        shdrs.append(sh(b".lay_subprog1", 1, 3, s2, places[1], len(img2), 0x10))
    shdrs.append(sh(b".shstrtab", 3, 0, 0, shstr_off, len(shstr), 1))
    flags = struct.unpack_from("<I", e.d, 0x24)[0]
    hdr = bytearray(b"\x7fELF\x01\x01\x01" + bytes(9))
    hdr += struct.pack("<HHIIIIIHHHHHH", 2, 8, 1, e.entry, phoff, shoff, flags, 0x34, 0x20, nph, 40,
                       len(shdrs), len(shdrs) - 1)
    data = bytearray(shoff + 40 * len(shdrs))
    data[:len(hdr)] = hdr
    phs = []
    for i, ((start, img, memsz), off) in enumerate(zip(segs, places)):
        flags_p = 7 if i == 0 else 6
        align = 0x80 if i == 0 else 0x10
        data[phoff + 0x20 * i:phoff + 0x20 * (i + 1)] = struct.pack(
            "<8I", 1, off, start, start, len(img), memsz, flags_p, align)
        data[off:off + len(img)] = img
        phs.append((start, len(img), memsz))
    data[shstr_off:shstr_off + len(shstr)] = shstr
    data[shoff:] = b"".join(shdrs)
    with open(os.path.join(ROOT, out_path), "wb") as f:
        f.write(data)
    return phs


def program_headers(path):
    d = open(os.path.join(ROOT, path), "rb").read()
    phoff = struct.unpack_from("<I", d, 0x1C)[0]
    phentsize, phnum = struct.unpack_from("<HH", d, 0x2A)
    return [struct.unpack_from("<8I", d, phoff + i * phentsize) for i in range(phnum)]


def loaded_bytes(path):
    """What the PS2 loads, as tools/elf2rom.py extracts it: PT_LOAD contents back to back."""
    d = open(os.path.join(ROOT, path), "rb").read()
    out = b""
    for typ, off, vaddr, paddr, filesz, memsz, flg, align in sorted(program_headers(path), key=lambda p: p[1]):
        if typ == 1 and filesz:
            out += d[off:off + filesz]
    return out


def objcopy_bin(elf, out):
    subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", elf, out], cwd=ROOT, check=True)


# ------------------------------------------------------------------ driver
def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("name")
    for b in ("main", "battle"):
        ap.add_argument(f"--{b}-pin", metavar="PADS")
    for b in ("main", "field", "battle", "viewer"):
        ap.add_argument(f"--{b}-pad", metavar="PADS")
    args = ap.parse_args()
    os.chdir(ROOT)
    spec = {}
    if args.name in PRESETS and not any(getattr(args, k) for k in vars(args) if k != "name"):
        raw = PRESETS[args.name]
    else:
        raw = {}
        for b in ("main", "field", "battle", "viewer"):
            pin = getattr(args, f"{b}_pin", None)
            pad = getattr(args, f"{b}_pad", None)
            if pin and pad:
                raise SystemExit(f"{b}: --{b}-pin or --{b}-pad, not both")
            if pin:
                raw[b] = ("pin", pin)
            elif pad:
                raw[b] = ("pad", pad)
    for b, (kind, pads) in raw.items():
        pads = parse_pads(pads)
        spec[b] = ("pin", dict(pads)) if kind == "pin" else ("pad", pads)

    out_dir = f"{OUT}/{args.name}"
    os.makedirs(out_dir, exist_ok=True)
    work = f"{out_dir}/link"
    os.makedirs(work, exist_ok=True)
    report = [f"# {args.name}: {', '.join(f'{b} {k} {p}' for b, (k, p) in raw.items()) or 'the normal build'}"]

    elfs = {}
    status = 0
    if spec:
        ok = shift_test.read_ok("config/shift_ok.txt")
        linker = shift_test.Linker(shift_test.DEFAULT_POINTS, out=work)
        results, elfs, case = linker.run(args.name, spec, ok, allow_known=True)
        for b, res in results.items():
            st, line = shift_test.summarize(args.name, b, res, case, True)
            status = max(status, st)
            report.append(line)
        shift_test.write_details(f"{out_dir}/check.txt", results)
        if status:
            raise SystemExit(f"{args.name}: the check failed (details in {out_dir}/check.txt); nothing written")
    for b in ("main", "field", "battle", "viewer"):
        elf = elfs.get(b) or f"build/{shift_test.elf_name(b)}.elf"
        dst = f"{out_dir}/{FILES[b]}"
        if b == "main":
            phs = write_ps2_elf(elf, dst)
            report.append(f"main ELF from {elf}: " + "; ".join(
                f"PT_LOAD {v:08X} filesz {fs:#x} memsz {ms:#x}" for v, fs, ms in phs))
            end = phs[0][0] + phs[0][2]
            report.append(f"main ends at {end:08X} ({0x005B0000 - end:#x} below FIELD)")
        else:
            objcopy_bin(elf, dst)
            size = os.path.getsize(os.path.join(ROOT, dst))
            vram = shift_test.BINARIES[b]["vram"]
            limit = {"field": 0x00680000, "battle": 0x006FFFFC, "viewer": 0x00750000}[b]  # VIEWER already covers the first GBX slots
            report.append(f"{FILES[b]}: {size:#x} bytes, {vram:08X}-{vram + size:08X} "
                          f"({limit - vram - size:#x} below {limit:08X})")
            if vram + size > limit:
                raise SystemExit(f"{FILES[b]} ends past {limit:08X}")
    # the main ELF: its PT_LOAD contents must be the linked image
    e = shift_test.Image(elfs.get("main") or "build/SLUS_204.17.elf")
    d = open(os.path.join(ROOT, f"{out_dir}/SLUS_204.17"), "rb").read()
    for typ, off, vaddr, paddr, filesz, memsz, flg, align in program_headers(f"{out_dir}/SLUS_204.17"):
        for lo, hi, s, sd in e.secs:
            if s["type"] == elf32.SHT_NOBITS or not vaddr <= lo < vaddr + memsz:
                continue
            if d[off + lo - vaddr:off + hi - vaddr] != sd:
                raise SystemExit(f"the ELF's PT_LOAD at {vaddr:08X} doesn't hold {s['name']}")
    orig = "iso/SLUS_204.17"
    if os.path.exists(os.path.join(ROOT, orig)):
        report.append("original program headers: " + "; ".join(
            f"{p[2]:08X}/{p[3]:08X} off {p[1]:#x} filesz {p[4]:#x} memsz {p[5]:#x} flags {p[6]} align {p[7]:#x}"
            for p in program_headers(orig)))
        report.append("new program headers:      " + "; ".join(
            f"{p[2]:08X}/{p[3]:08X} off {p[1]:#x} filesz {p[4]:#x} memsz {p[5]:#x} flags {p[6]} align {p[7]:#x}"
            for p in program_headers(f"{out_dir}/SLUS_204.17")))
    if args.name == "control":
        same = loaded_bytes(f"{out_dir}/SLUS_204.17") == open(os.path.join(ROOT, "rom/SLUS_204.17.rom"), "rb").read()
        report.append(f"control: loaded bytes {'identical to' if same else 'DIFFER from'} rom/SLUS_204.17.rom")
        if not same:
            raise SystemExit("control: the ELF doesn't load the original bytes")
        for b in ("field", "battle", "viewer"):
            same = open(os.path.join(ROOT, f"{out_dir}/{FILES[b]}"), "rb").read() == \
                open(os.path.join(ROOT, f"iso/{FILES[b]}"), "rb").read()
            report.append(f"control: {FILES[b]} {'identical to' if same else 'DIFFERS from'} the disc's")
            if not same:
                raise SystemExit(f"control: {FILES[b]} differs")
    for b in ("main", "field", "battle", "viewer"):
        p = os.path.join(ROOT, f"{out_dir}/{FILES[b]}")
        report.append(f"{FILES[b]:12} {os.path.getsize(p):8} bytes sha1 {hashlib.sha1(open(p, 'rb').read()).hexdigest()}")
    with open(os.path.join(ROOT, f"{out_dir}/report.txt"), "w") as f:
        f.write("\n".join(report) + "\n")
    print("\n".join(report[1:] if spec else report))
    print(f"wrote {out_dir}/")
    sys.exit(0)


if __name__ == "__main__":
    main()
