#!/usr/bin/env python3
"""Check that every symbol the built binaries define is at its original address.

Usage: check_syms.py [binary...]   (after make; default: all four)

The original address of a name is the one in config/symbol_addrs*.txt, or the
address in a D_XXXXXXXX / func_XXXXXXXX name. For each binary, every such
name inside its memory that its built ELF defines must have that value.
Byte matching already proves this for every symbol code or data refers to;
this also covers the ones nothing refers to (unreferenced .bss objects, the
names tools/c_syms.py defines relative to an object), which matters once the
data is in C (plan step 6a). Exit status 1 on any difference.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elf32  # noqa: E402
from c_syms import MEMORY, in_memory, symbol_file_names  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
ADDR_NAME_RE = re.compile(r"(?:D|func)_([0-9A-F]{8})")


def main():
    os.chdir(ROOT)
    binaries = sys.argv[1:] or list(MEMORY)
    bad = 0
    for b in binaries:
        files = ["config/symbol_addrs.txt"] if b == "main" else \
            [f"config/symbol_addrs.{b}.txt", "config/symbol_addrs.txt"]
        named = symbol_file_names([f for f in files if os.path.exists(f)])
        elf = elf32.Elf(f"build/{'SLUS_204.17' if b == 'main' else b}.elf")
        checked = 0
        for s in elf.symbols():
            name = s["name"]
            if not name or s["shndx"] == elf32.SHN_UNDEF:
                continue
            m = ADDR_NAME_RE.fullmatch(name)
            want = int(m.group(1), 16) if m else named.get(name)
            if want is None or not in_memory(b, want):
                continue
            checked += 1
            if s["value"] != want:
                print(f"{b}: {name} is at 0x{s['value']:08X}, originally 0x{want:08X}")
                bad += 1
        print(f"{b}: {checked} symbols checked")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
