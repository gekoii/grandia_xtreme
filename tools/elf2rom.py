#!/usr/bin/env python3
"""Extract the loadable bytes of a PS2 ELF into a flat "rom" for splat.

The rom is the file contents of each PT_LOAD program segment, back to back in
file order. That is exactly what the PS2 copies into memory, and it leaves out
ELF file padding between segments, which splat would otherwise treat as data.

Accepts objcopy-style arguments so it can be passed to
`splat create_config --objcopy`; only the last two (ELF, output) are used.
"""
import struct
import sys

PT_LOAD = 1


def main() -> None:
    if "--version" in sys.argv:
        print("elf2rom 2.0")
        return
    elf_path, rom_path = sys.argv[-2], sys.argv[-1]
    data = open(elf_path, "rb").read()
    phoff = struct.unpack_from("<I", data, 0x1C)[0]
    phentsize, phnum = struct.unpack_from("<HH", data, 0x2A)

    segments = []
    for i in range(phnum):
        typ, off, _, _, filesz = struct.unpack_from("<5I", data, phoff + i * phentsize)
        if typ == PT_LOAD and filesz:
            segments.append((off, filesz))

    with open(rom_path, "wb") as f:
        for off, filesz in sorted(segments):
            f.write(data[off : off + filesz])


if __name__ == "__main__":
    main()
