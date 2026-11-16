"""The disc's MIPS programs (port/src/programs.c, port/src/mips/,
port/include/grx_mips.h).

1. Calls. The game calls the programs at their EE addresses as C
   functions; those calls go to the interpreter, grx_mips_call(addr, a0):
   BtlDataLoad's entries of the GBX programs (b:6BC620, (EntFn)0x708000-
   0x740000) and MgcUpdate's entry of the magic or skill program
   (b:6B1290, func_00748000(m)). The function pointers the programs store
   into the game's structs (MgcWork.update, BtlCharDef.init, ...) are
   native thunks the interpreter hands out, so the game's own calls
   through them need nothing here.

   These two were in port/tools/build.py's FIXUPS; the patterns accept
   the text before and after them, so the build works with or without
   those (they should go from build.py).

2. Loads. Each copy or read of a program into its load area goes to the
   interpreter's window and tells it a new program is there:
   FldBattleStart (f:619C80) copies the GBX programs out of sysGbxPac,
   DbgBattleSetup (m:41FE40) reads the debug GBX files, MgcUpdate copies
   entry 9 of a BTL_DATA pack to 0x748000, MgcLoadFile (b:6B19E0) reads a
   BATTLE/*.GBX file there (debug).
"""

import re

_DECLS = (r"\A", "void *grx_mips_area_host(unsigned int); void *grx_mips_load(void *, const void *, int); "
                 "void *grx_mips_load_dest(unsigned int); int grx_mips_call(unsigned int, unsigned int);\n#line 1\n")


def _lit(old, new):
    return (re.escape(old), new.replace("\\", "\\\\"))


def _gbx_names(kind, base):
    """dst[i] = btl<kind>Gbx<i>; -> the window's host address of that slot"""
    return [_lit(f"dst[{i}] = btl{kind}Gbx{i};", f"dst[{i}] = grx_mips_area_host(0x{base + i * 0x8000:X});")
            for i in range(4)]


def _gbx_reads(kind, base):
    return [_lit(f"FileRead(buf, (s32)btl{kind}Gbx{i});",
                 f"FileRead(buf, (s32)grx_mips_load_dest(0x{base + i * 0x8000:X}));")
            for i in range(4)]


FIXUPS = {
    # BtlDataLoad: the GBX programs' entries
    "src/battle/6BC620.c": [
        _DECLS,
        (r"(?:fn\[(i|i - 4)\]\(e\);|grx_mips_call\(\(unsigned int\)fn\[(i|i - 4)\], \(unsigned int\)e\);)",
         lambda m: f"grx_mips_call((unsigned int)fn[{m.group(1) or m.group(2)}], (unsigned int)e);"),
    ],
    # MgcUpdate: entry 9 of the pack copied to 0x748000, and its entry
    "src/battle/6B1290.c": [
        _DECLS,
        _lit("MemMove128(func_00748000, BtlPakGetEntry(r, 9), BtlPakGetEntrySize(r, 9) / 16);",
             "grx_mips_load(grx_mips_area_host(0x748000), BtlPakGetEntry(r, 9), BtlPakGetEntrySize(r, 9) / 16);"),
        (r"(?:\bfunc_00748000\(m\);|grx_mips_call\(0x748000, \(unsigned int\)m\);)",
         "grx_mips_call(0x748000, (unsigned int)m);"),
    ],
    # MgcLoadFile: a debug GBX file read straight to 0x748000
    "src/battle/6B19E0.c": [
        _DECLS,
        _lit("BtlFileLoadAsync(buf, func_00748000);", "BtlFileLoadAsync(buf, (s32)grx_mips_load_dest(0x748000));"),
    ],
    # FldBattleStart: the GBX programs copied out of sysGbxPac
    "src/field/619C80.c": [_DECLS] + _gbx_names("Party", 0x708000) + _gbx_names("Enemy", 0x728000) + [
        _lit("MemMove128(dst[i], (void *)sysGbxPac->ofs[id], (sysGbxPac->ofs[id + 1] - sysGbxPac->ofs[id]) >> 4);",
             "grx_mips_load(dst[i], (void *)sysGbxPac->ofs[id], (sysGbxPac->ofs[id + 1] - sysGbxPac->ofs[id]) >> 4);"),
        _lit("MemMove128(dst[i], (void *)sysGbxPac->ofs[m], (sysGbxPac->ofs[m + 1] - sysGbxPac->ofs[m]) >> 4);",
             "grx_mips_load(dst[i], (void *)sysGbxPac->ofs[m], (sysGbxPac->ofs[m + 1] - sysGbxPac->ofs[m]) >> 4);"),
    ],
    # DbgBattleSetup: the debug GBX files
    "src/main/41FE40.c": [_DECLS] + _gbx_reads("Enemy", 0x728000) + _gbx_reads("Party", 0x708000),

}

# 3. Not about the programs, but what kept battles from running once they
#    ran (to move to the objects/ABI owner's module): 12-byte vectors on the
#    stack that callees store a whole quadword into. BtlChrGetPartPos,
#    BtlChrPartToWorld, the Vu* helpers store with sqc2/sq; on the EE the
#    fourth word of a VuVec3 or f32[3] local landed in the frame's padding,
#    in the port it lands on whatever is next: BtlChrGetCenterY's return
#    address (EnvShadowDraw returned to 0x3F800000, 1.0f, the w of the
#    position) and FxCastAuraRegister's saved %esi (FxUnitRegisterModels
#    then read through 0x3F800000). Every VuVec3 / f32[3] local whose
#    address is handed to a function (26, found by a scan of src/: the
#    local is used only as v.x/v.y/v.z and &v, or indexed) gets 16 bytes:
#    a VuVec3 becomes a VuVec4 (same x, y, z), an f32[3] an f32[4]. libm's
#    __kernel_rem_pio2 (m:47DFE0, host-replaced) is left alone.
_QUAD_LOCALS = [
    ("src/battle/6823C8.c", "VuVec3", "v"), ("src/battle/684CB0.c", "VuVec3", "v"),
    ("src/battle/6996F8.c", "VuVec3", "v"), ("src/battle/69B0F0.c", "VuVec3", "v"),
    ("src/battle/69B3F0.c", "VuVec3", "v"), ("src/battle/69B998.c", "VuVec3", "v"),
    ("src/battle/6A1398.c", "VuVec3", "v"), ("src/battle/6AB120.c", "VuVec3", "v"),
    ("src/battle/6AB498.c", "VuVec3", "v"), ("src/battle/6BFC30.c", "VuVec3", "v"),
    ("src/battle/6C6460.c", "f32[3]", "v"), ("src/battle/6C7620.c", "VuVec3", "v"),
    ("src/battle/6C98A8.c", "VuVec3", "v"), ("src/battle/6C9F78.c", "VuVec3", "v"),
    ("src/field/5B68B0.c", "VuVec3", "v"), ("src/field/5B6D98.c", "f32[3]", "v"),
    ("src/field/5B73A8.c", "f32[3]", "v"), ("src/field/5B7B50.c", "f32[3]", "v"),
    ("src/field/5B8138.c", "f32[3]", "v"), ("src/field/5B8908.c", "f32[3]", "v"),
    ("src/field/5B8D78.c", "f32[3]", "v"), ("src/field/5BB168.c", "VuVec3", "v"),
    ("src/field/5C2A68.c", "VuVec3", "v"),
    ("src/main/441F10.c", "VuVec3", "a"), ("src/main/441F10.c", "VuVec3", "b"),
]
for _rel, _kind, _name in _QUAD_LOCALS:
    if _kind == "VuVec3":
        _sub = (r"(?m)^([ \t]+)VuVec3([ \t]+" + _name + r");",
                r"\1VuVec4\2; /* port: room for a quadword store (mips.py) */")
    else:
        _sub = (r"(?m)^([ \t]+)f32([ \t]+" + _name + r")\[3\];",
                r"\1f32\2[4]; /* port: room for a quadword store (mips.py) */")
    FIXUPS.setdefault(_rel, []).append(_sub)
