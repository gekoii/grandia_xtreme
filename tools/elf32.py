"""A small reader for 32-bit little-endian ELF files (the game's binaries and
our objects): sections, symbols and REL/RELA relocations. Pure Python, no
binutils needed.
"""
import struct

SHT_SYMTAB = 2
SHT_RELA = 4
SHT_NOBITS = 8
SHT_REL = 9
SHF_ALLOC = 2
SHN_UNDEF = 0
SHN_ABS = 0xFFF1
SHN_COMMON = 0xFFF2

R_MIPS_32 = 2
R_MIPS_26 = 4
R_MIPS_HI16 = 5
R_MIPS_LO16 = 6
RELOC_NAMES = {0: "NONE", 2: "32", 4: "26", 5: "HI16", 6: "LO16", 7: "GPREL16"}


class Elf:
    def __init__(self, path):
        self.path = path
        d = self.d = open(path, "rb").read()
        if d[:4] != b"\x7fELF" or d[4] != 1 or d[5] != 1:
            raise ValueError(f"{path}: not a 32-bit little-endian ELF")
        (self.e_type, _, _, self.entry, self.phoff, self.shoff, _, _, _, _,
         self.shentsize, self.shnum, self.shstrndx) = struct.unpack_from("<HHIIIIIHHHHHH", d, 16)
        self.sections = []
        for i in range(self.shnum):
            f = struct.unpack_from("<IIIIIIIIII", d, self.shoff + i * self.shentsize)
            self.sections.append(dict(name_off=f[0], type=f[1], flags=f[2], addr=f[3], offset=f[4],
                                      size=f[5], link=f[6], info=f[7], align=f[8], entsize=f[9], idx=i))
        strtab = self.sections[self.shstrndx]
        for s in self.sections:
            s["name"] = self._str(strtab, s["name_off"])
        self._syms = None

    def _str(self, sec, off):
        o = sec["offset"] + off
        return self.d[o:self.d.index(b"\0", o)].decode("latin1")

    def section(self, name):
        for s in self.sections:
            if s["name"] == name:
                return s
        return None

    def data(self, sec):
        if sec["type"] == SHT_NOBITS:
            return bytes(sec["size"])
        return self.d[sec["offset"]:sec["offset"] + sec["size"]]

    def alloc_sections(self):
        """Sections that occupy memory when loaded (with or without file contents)."""
        return [s for s in self.sections if s["flags"] & SHF_ALLOC and s["size"]]

    def symbols(self):
        """Every symbol: dicts with name, value, size, bind, type, shndx (index 0 included)."""
        if self._syms is None:
            self._syms = []
            for s in self.sections:
                if s["type"] != SHT_SYMTAB:
                    continue
                strs = self.sections[s["link"]]
                for i in range(s["size"] // 16):
                    nm, val, size, info, other, shndx = struct.unpack_from("<IIIBBH", self.d, s["offset"] + i * 16)
                    self._syms.append(dict(name=self._str(strs, nm), value=val, size=size,
                                           bind=info >> 4, type=info & 15, shndx=shndx, index=i))
        return self._syms

    def defined(self):
        """name -> symbol for every named, defined symbol (first one wins)."""
        out = {}
        for s in self.symbols():
            if s["name"] and s["shndx"] != SHN_UNDEF:
                out.setdefault(s["name"], s)
        return out

    def relocs(self, target=None):
        """Relocations as (offset, type, symbol dict, addend or None, target section name),
        for every relocation section or only those applying to section `target`."""
        out = []
        syms = self.symbols()
        for s in self.sections:
            if s["type"] not in (SHT_REL, SHT_RELA):
                continue
            tname = self.sections[s["info"]]["name"]
            if target is not None and tname != target:
                continue
            ent = 8 if s["type"] == SHT_REL else 12
            for i in range(s["size"] // ent):
                if ent == 8:
                    off, info = struct.unpack_from("<II", self.d, s["offset"] + i * 8)
                    add = None
                else:
                    off, info, add = struct.unpack_from("<IIi", self.d, s["offset"] + i * 12)
                out.append((off, info & 0xFF, syms[info >> 8], add, tname))
        return out
