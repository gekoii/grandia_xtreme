#!/usr/bin/env python3
"""Check whether the functions in one C file match the original binary.

The binary (main executable or an overlay, see tools/binaries.py) is the one
whose code holds the file's first function.

Safe to run many times in parallel: it works in a private temp directory and
never touches src/, config/, asm/ or the shared build. Run inside the build
container:

    tools/docker.sh python3 tools/check_func.py path/to/0040EDF8.c

For every func_XXXXXXXX defined in the file it prints OK, or the differing
instructions side by side (compiled vs original), with relocations resolved.
The file's .rodata is compared too: its jump tables (config/jtbls.<binary>.txt),
or the range a `/* rodata: 0xSTART-0xEND */` marker claims for constants and
strings. tools/update_segments.py places it there in the real build.
Exit status is 0 only if everything matches.
"""
import os
import re
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from compile_c import compile_c, owned_rodata  # noqa: E402
from m2c_batch import SYMBOL_ADDRS, run, sext16, symbol_addr  # noqa: E402
from binaries import binary_for_addr  # noqa: E402

DEF_RE = re.compile(r"^[A-Za-z_].*?\b(\w+)\(.*\)\s*\{\s*$", re.M)


def defined_functions(text):
    """[(name, address)] of the functions a C file defines: func_XXXXXXXX, or a
    real name from config/symbol_addrs.txt (e.g. sceMpegGetPicture)."""
    out = []
    for name in DEF_RE.findall(text):
        if re.fullmatch(r"func_[0-9A-F]{8}", name):
            out.append((name, int(name[5:], 16)))
        elif name in SYMBOL_ADDRS:
            out.append((name, SYMBOL_ADDRS[name]))
    return out


def load_tables(path):
    tables = {}
    if not os.path.exists(path):
        return tables
    for line in open(path):
        if line.strip() and not line.startswith("#"):
            func, addr, size = line.split()
            tables.setdefault(int(func, 16), []).append((int(addr, 16), int(size, 16)))
    return tables


def section_bytes(obj, section):
    return subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", section, obj, "/dev/stdout"],
                          capture_output=True).stdout


def relocations(obj):
    """{section: {offset: (type, symbol)}} from readelf."""
    out, section = {}, None
    for line in run(["mips-linux-gnu-readelf", "-rW", obj]).stdout.splitlines():
        if line.startswith("Relocation section"):
            section = line.split("'")[1].replace(".rel", "", 1)
            out[section] = {}
            continue
        parts = line.split()
        if section and len(parts) >= 3 and re.fullmatch(r"[0-9a-f]{8}", parts[0]):
            out[section][int(parts[0], 16)] = (parts[2], parts[4] if len(parts) > 4 else "")
    return out


def symbols(obj):
    """{name: (offset, size)} for function symbols in .text."""
    out = {}
    for line in run(["mips-linux-gnu-nm", "-S", obj]).stdout.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[2] in "Tt":
            out[parts[3]] = (int(parts[0], 16), int(parts[1], 16))
    return out


def resolve(word, rtype, sym, words, relocs, off, section_base):
    """Value of a relocated word once linked, or None if it can't be resolved.
    MIPS REL relocations keep their addend in the instruction/word."""
    addr = section_base.get(sym, symbol_addr(sym))
    if addr is None:
        return None
    if rtype == "R_MIPS_32":
        return (addr + word) & 0xFFFFFFFF
    if rtype == "R_MIPS_26":
        return (word & 0xFC000000) | (((addr + ((word & 0x03FFFFFF) << 2)) >> 2) & 0x03FFFFFF)
    if rtype == "R_MIPS_LO16":
        return (word & 0xFFFF0000) | ((addr + sext16(word)) & 0xFFFF)
    if rtype == "R_MIPS_HI16":
        lo = 0
        for o in sorted(relocs):
            if o > off and relocs[o] == ("R_MIPS_LO16", sym):
                lo = words.get(o, 0)
                break
        full = addr + ((word & 0xFFFF) << 16) + sext16(lo)
        return (word & 0xFFFF0000) | (((full + 0x8000) >> 16) & 0xFFFF)
    return None


def disasm_words(words, vram, tmp):
    """objdump the given words as R5900 code; returns one string per word."""
    raw = os.path.join(tmp, "words.bin")
    with open(raw, "wb") as f:
        f.write(struct.pack("<%dI" % len(words), *words))
    # -z: show runs of zero words (nops) instead of collapsing them to "..."
    out = run(["mips-linux-gnu-objdump", "-D", "-z", "-b", "binary", "-m", "mips:5900", "-EL",
               f"--adjust-vma=0x{vram:X}", raw]).stdout
    lines = re.findall(r"^\s*[0-9a-f]+:\s+[0-9a-f]{8}\s+(.*)$", out, re.M)
    return [re.sub(r"\s+", " ", l).strip() for l in lines]


def main():
    c_file = sys.argv[1]
    funcs = defined_functions(open(c_file).read())
    if not funcs:
        sys.exit(f"{c_file}: no func_XXXXXXXX (or named) function definitions")
    addrs = sorted(a for _, a in funcs)
    binary = binary_for_addr(addrs[0])
    VRAM = binary["vram"]
    starts = sorted(int(l, 16) for l in open(binary["functions"]) if l.strip())
    starts.append(VRAM + binary["text_end"])
    all_tables = load_tables(binary["jtbls"])
    rom = open(binary["rom"], "rb").read()
    file_tables = sorted(t for a in addrs for t in all_tables.get(a, []))
    # The .rodata this file owns: an explicit `/* rodata: */` marker range
    # (constants, strings, tables), or else just its jump tables.
    owned = owned_rodata(open(c_file).read())
    if not owned and file_tables:
        owned = (file_tables[0][0], file_tables[-1][0] + file_tables[-1][1])

    all_ok = True
    with tempfile.TemporaryDirectory() as tmp:
        # Same compile path as the Makefile, including per-file toolchain markers
        obj = os.path.join(tmp, "out.o")
        ok, err = compile_c(c_file, obj)
        if not ok:
            sys.exit(err)

        text = section_bytes(obj, ".text")
        text_words = {i * 4: w for i, w in enumerate(struct.unpack("<%dI" % (len(text) // 4), text))}
        relocs = relocations(obj)
        syms = symbols(obj)
        # Section symbols: this file's .text starts at its first function, and
        # its .rodata (jump tables) at its first table.
        section_base = {".text": addrs[0]}
        if owned:
            section_base[".rodata"] = owned[0]

        for name, addr in funcs:
            if addr not in starts or name not in syms:
                print(f"{name}: not a function start in {binary['functions']}")
                all_ok = False
                continue
            off, size = syms[name]
            n_orig = (starts[starts.index(addr) + 1] - addr) // 4
            orig = list(struct.unpack_from("<%dI" % n_orig, rom, addr - VRAM))
            got = []
            for o in range(off, off + size, 4):
                w = text_words[o]
                if o in relocs.get(".text", {}):
                    r = resolve(w, *relocs[".text"][o], text_words, relocs[".text"], o, section_base)
                    w = r if r is not None else w
                got.append(w)
            real = n_orig
            while real > len(got) and orig[real - 1] == 0:
                real -= 1
            orig = orig[:real]
            bad = [i for i in range(max(len(got), len(orig)))
                   if i >= len(got) or i >= len(orig) or got[i] != orig[i]]
            if not bad:
                print(f"{name}: OK")
                continue
            all_ok = False
            print(f"{name}: {len(bad)} differing words (compiled {len(got)} insns, original {len(orig)})")
            g_txt = disasm_words(got, addr, tmp)
            o_txt = disasm_words(orig, addr, tmp)
            for i in range(max(len(got), len(orig))):
                mark = "!" if i in bad else " "
                g = g_txt[i] if i < len(g_txt) else ""
                o = o_txt[i] if i < len(o_txt) else ""
                print(f" {mark} {addr + 4 * i:08X}  {g:<40} | {o}")

        # .rodata: compare what the file emits with the original range it owns
        rodata = section_bytes(obj, ".rodata")
        what = (f"{len(file_tables)} jump table{'s' if len(file_tables) != 1 else ''}"
                if file_tables else "constants")
        if owned or rodata:
            if not owned:
                print(f"rodata: the file emits {len(rodata)} bytes of .rodata but owns none "
                      f"(add a /* rodata: 0xSTART-0xEND */ marker for constants or strings)")
                all_ok = False
            else:
                base, want_len = owned[0], owned[1] - owned[0]
                want_bytes = rom[base - VRAM:base - VRAM + want_len]
                # A string can leave .rodata a few bytes short of a whole word;
                # compare padded with zeros, like the alignment padding after it
                padded = rodata + b"\0" * (-len(rodata) % 4)
                words = {i * 4: w for i, w in enumerate(struct.unpack("<%dI" % (len(padded) // 4), padded))}
                bad = 0
                for o, w in sorted(words.items()):
                    if o in relocs.get(".rodata", {}):
                        r = resolve(w, *relocs[".rodata"][o], words, relocs[".rodata"], o, section_base)
                        w = r if r is not None else w
                    want = struct.unpack_from("<I", want_bytes, o)[0] if o + 4 <= want_len else None
                    if w != want:
                        bad += 1
                # Shorter is fine only if the rest of the range is alignment padding
                short_ok = len(rodata) <= want_len and not any(want_bytes[len(rodata):])
                if not short_ok:
                    print(f"rodata: {len(rodata):#x} bytes emitted, the owned range is {want_len:#x}")
                    all_ok = False
                elif bad:
                    print(f"rodata: {bad} words differ ({what})")
                    all_ok = False
                else:
                    print(f"rodata: OK ({what})")
    sys.exit(0 if all_ok else 1)


if __name__ == "__main__":
    main()
