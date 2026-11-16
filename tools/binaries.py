"""Where each of the game's four binaries lives and how it is laid out.

The main executable and the three code overlays are split and matched the
same way; the tools take a binary name and look its layout up here.

Offsets are into the flat image the tools compare against (rom/ for the main
executable, the raw overlay file for the others): offset = address - vram.
"""

BINARIES = {
    "main": {
        "config": "config/SLUS_204.17.yaml",
        "rom": "rom/SLUS_204.17.rom",
        "vram": 0x00400000,
        "text_end": 0x095A60,  # where .vutext starts
        "data": (0x099B00, 0x0A1480),  # the .data section (after .text_nop), up to .rodata
        "rodata": (0x0A1480, 0x0A9410),  # the .rodata section
        # Section starts (vram), from the ELF's section headers
        "sections": {"data": 0x00499A70, "rodata": 0x004A1480, "scommon": 0x004A9410,
                     "bss": 0x004A9680, "end": 0x005A6B20},
    },
    "field": {
        "config": "config/field.yaml",
        "rom": "iso/FIELD.BIN",
        "vram": 0x005B0000,
        "text_end": 0x0B6080,
        "data": (0x0B6080, 0x0C0BF0),  # the .data section
        "rodata": (0x0C0BF0, 0x0C62B8),  # the .rodata section
        # Section starts (vram), settled in plan step 5: see docs/shifting.md
        "sections": {"data": 0x00666080, "rodata": 0x00670BF0, "scommon": 0x006762B8,
                     "bss": 0x00676480, "end": 0x0067F646},
    },
    "battle": {
        "config": "config/battle.yaml",
        "rom": "iso/BATTLE.BIN",
        "vram": 0x00680000,
        "text_end": 0x05EC00,
        "data": (0x05EC00, 0x05F500),
        "rodata": (0x05F500, 0x063E80),
        "sections": {"data": 0x006DEC00, "rodata": 0x006DF500, "scommon": 0x006E3E80,
                     "bss": 0x006E3F00, "end": 0x006F9350},
    },
    "viewer": {
        "config": "config/viewer.yaml",
        "rom": "iso/VIEWER.BIN",
        "vram": 0x00700000,
        "text_end": 0x003380,
        "data": None,  # VIEWER has no .data
        "rodata": (0x003380, 0x0034D0),
        # no .data; .bss starts 128-aligned at 0x00703500 after a 0x30-byte gap
        "sections": {"data": 0x00703380, "rodata": 0x00703380, "bss": 0x007034D0, "end": 0x0070DF44},
    },
}

for _name, _b in BINARIES.items():
    _b["name"] = _name
    _b["functions"] = f"config/functions.{_name}.txt"  # start address of every function
    _b["jtbls"] = f"config/jtbls.{_name}.txt"  # which function owns which jump table
    _b["src"] = f"src/{_name}"
    _b["asm_code"] = f"asm/{_name}/{_name}"
    _b["asm_data"] = f"asm/{_name}/data/{_name}"


def binary_for_addr(addr):
    """The binary whose code contains this address (the four don't overlap)."""
    for b in BINARIES.values():
        if b["vram"] <= addr < b["vram"] + b["text_end"]:
            return b
    raise KeyError(f"0x{addr:08X} is not in any binary's code")


def binary_for_path(path):
    """The binary a C file belongs to, from a src/<binary>/ path, else None."""
    parts = path.replace("\\", "/").split("/")
    if "src" in parts:
        name = parts[parts.index("src") + 1]
        if name in BINARIES:
            return BINARIES[name]
    return None
