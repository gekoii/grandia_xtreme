#!/usr/bin/env python3
"""Compile a C file with every candidate compiler and compare its functions
against the original code (run inside the build container).

Usage: probe_compilers.py <file.c> <rom> <rom vram base> [opt flags...]
Functions must be named func_XXXXXXXX after their original address and must
not contain relocations (they are compared word for word).
"""
import os
import re
import struct
import subprocess
import sys

COMPILERS = ["ee-gcc2.9-990721", "ee-gcc2.9-991111", "ee-gcc2.9-991111a",
             "ee-gcc2.9-991111-01", "ee-gcc2.96"]


def function_words(obj):
    """Return {name: [words]} for each function symbol in an object file."""
    syms = subprocess.run(["mips-linux-gnu-nm", "-S", obj], capture_output=True, text=True).stdout
    text = subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", obj, "/dev/stdout"],
                          capture_output=True).stdout
    out = {}
    for line in syms.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[2] in "Tt":
            off, size = int(parts[0], 16), int(parts[1], 16)
            out[parts[3]] = list(struct.unpack_from("<%dI" % (size // 4), text, off))
    return out


def main():
    c_file, rom_path, base = sys.argv[1], sys.argv[2], int(sys.argv[3], 16)
    opts = sys.argv[4:] or ["-O2"]
    rom = open(rom_path, "rb").read()
    obj = "/tmp/probe.o"
    for comp in COMPILERS:
        for opt in opts:
            asm = "/tmp/probe.s"
            r = subprocess.run([f"tools/compilers/{comp}/bin/ee-gcc", opt, "-G0", "-S", "-o", asm, c_file],
                               capture_output=True, text=True)
            if r.returncode:
                print(f"{comp:22} {opt}: compile error {r.stderr.strip()[:200]}")
                continue
            # Bundled EE assembler by default; PROBE_GAS=1 uses modern GNU as instead.
            if os.environ.get("PROBE_GAS"):
                as_cmd = ["mips-linux-gnu-as", "-EL", "-march=r5900", "-mabi=eabi", "-G0", "-no-pad-sections"]
            else:
                as_cmd = [f"tools/compilers/{comp}/ee/bin/as", "-EL"]
            r = subprocess.run(as_cmd + ["-o", obj, asm], capture_output=True, text=True)
            if r.returncode:
                print(f"{comp:22} {opt}: assemble error {r.stderr.strip()[:200]}")
                continue
            results = []
            for name, words in sorted(function_words(obj).items()):
                m = re.match(r"func_([0-9A-F]{8})$", name)
                if not m:
                    continue
                off = int(m.group(1), 16) - base
                want = list(struct.unpack_from("<%dI" % len(words), rom, off))
                diff = sum(a != b for a, b in zip(words, want))
                results.append(f"{name[5:]}:{'OK' if diff == 0 else str(diff) + 'diff'}")
            print(f"{comp:22} {opt}: " + " ".join(results))


if __name__ == "__main__":
    main()
