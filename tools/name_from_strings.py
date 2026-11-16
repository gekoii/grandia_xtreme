#!/usr/bin/env python3
"""Suggest function names from the debug strings each function references.

Many SDK functions print messages that start with their own name, e.g.
"sceGsSyncPath: GIF does not terminate". For every function in a splat asm
file, this collects the strings it loads (%hi/%lo pairs on D_ symbols) and,
when every name-like prefix in those strings agrees, suggests that name.

Usage: name_from_strings.py <asm file> <rom file> <rom vram base>
Prints suggestions in symbol_addrs.txt format; review them before use.
"""
import re
import sys
from collections import defaultdict

FUNC_RE = re.compile(r"^glabel (\w+)")
ADDR_RE = re.compile(r"^\s*/\* \w+ ([0-9A-F]{8}) ")
SYM_RE = re.compile(r"%lo\(D_([0-9A-F]+)\)")
# "sceFoo: ...", "sceFoo(...", "sceFoo() ..." -- an identifier followed by ':' or '('
NAME_RE = re.compile(r"^\??([A-Za-z_][A-Za-z0-9_]{3,})\s*[:(]")


def read_cstring(rom: bytes, off: int) -> str:
    if not 0 <= off < len(rom):
        return ""
    end = rom.find(b"\0", off, off + 256)
    if end <= off:
        return ""
    try:
        return rom[off:end].decode("ascii")
    except UnicodeDecodeError:
        return ""


def main() -> None:
    asm_path, rom_path, base = sys.argv[1], sys.argv[2], int(sys.argv[3], 16)
    rom = open(rom_path, "rb").read()

    func_addr = {}
    refs = defaultdict(set)
    current = None
    for line in open(asm_path):
        m = FUNC_RE.match(line)
        if m:
            current = m.group(1)
            continue
        if current is None:
            continue
        if current not in func_addr:
            a = ADDR_RE.match(line)
            if a:
                func_addr[current] = int(a.group(1), 16)
        for sym in SYM_RE.findall(line):
            refs[current].add(int(sym, 16))

    for func, addrs in refs.items():
        if not func.startswith("func_"):
            continue
        names = set()
        for addr in addrs:
            m = NAME_RE.match(read_cstring(rom, addr - base))
            if m:
                names.add(m.group(1))
        if len(names) == 1:
            name = names.pop()
            print(f"{name} = 0x{func_addr[func]:08X}; // type:func  from string")


if __name__ == "__main__":
    main()
