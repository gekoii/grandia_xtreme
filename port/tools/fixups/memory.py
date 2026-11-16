"""The game's memory at fixed addresses, and its uncached aliases
(port/src/memory.c, port/include/grx_memory.h, port/docs/hardware.md).

1. Fixed RAM. The literal main-RAM addresses the game's C uses for heaps
   and work areas become host allocations below 0x08000000 that keep the
   PS2's layout (grx_ee_area, grx_ee_ram):

   | PS2 address | Size | Used by | Area |
   | --- | --- | --- | --- |
   | 0x00760000-0x01FF0000 | 0x1890000 | MemInit(7, ...) in SysMain (m:4000C0) | GRX_AREA_HEAP7 |
   | 0x00200000 | 1 MB | MemHeapCreateAt, sysState.battleHeap (m:4000C0) | GRX_AREA_BATTLE_HEAP |
   | 0x00300000 | 1 MB | MemHeapCreateAt, sysState.faceHeap (m:4000C0) | GRX_AREA_FACE_HEAP |
   | 0x00100000 | 1 MB | FileRead of GRX.INI and its parse, SysIniLoad (m:40D860) | GRX_AREA_INI_BUF |
   | 0x00100000 | 1 MB | the MFIFO ring, SysMfifoStart (m:42FFC8) | GRX_AREA_MFIFO_RING |
   | 0x00770000 | (in the arena) | the debug memory view's default base, DbgViewInit (m:420808) | grx_ee_ram |
   | 0x01000000 | (in the arena) | the debug memory view's fallback, DbgMemDumpPage (m:42B440) | grx_ee_ram |

   SndFsMount's sector buffer at 0x01000000 (in the arena's range, used by
   SysBoot before MemInit, m:42FB00) is the SIF work's (sif.py,
   grx_sif_dir_scratch); grx_ee_ram(0x01000000) would give the same
   offset into the arena.

   Left alone: the EE/IOP block 0x01FF0000-0x02000000 (grx_hwmem_init
   maps it at its own address, with views at 0x21FF0000/0x31FF0000):
   LoadIopModules (m:45EEB8: 0x21FF0000, 0x21FF0100), SndFsMount's
   directory tree at 0x01FF1000 (m:42FB00), SndIopStatusRead (m:45F578:
   0x01FF0100). And the load areas of the disc's programs, which belong to
   the MIPS interpreter: BtlDataLoad (b:6BC620, (EntFn)0x708000-0x740000),
   MgcUpdate (b:6B1290) and MgcLoadFile (b:6B19E0, func_00748000),
   DbgBattleSetup (m:41FE40) and FldBattleStart (f:619C80, the
   btlPartyGbx*/btlEnemyGbx* names), sysProgramTable (m:499B00,
   FIELD_VRAM/BATTLE_VRAM/SUBPROG_VRAM), btlDbgStage (0x006FFFFC, a
   buffer).

2. Uncached aliases. `addr | 0x20000000` (uncached) and `| 0x30000000`
   (uncached-accelerated) become `| GRX_UNCACHED` / `| GRX_UNCACHED_ACC`,
   which are 0: on the host the uncached view is the cached one. Only at
   the alias sites hw_survey.py marks (plus sceWrite's `addr |=
   0x20000000`, which it misses), never at look-alikes: IPU command words
   (m:4762C0, m:476CB8), DMA tag IDs (m:4572C0 `tq |= 0x30000000`), the
   CHCR value in viBufAddDMA (m:466848), debug flag bits, libm's
   SET_FLOAT_WORD. Each pattern is the site's own text.

   Not done here, since their libraries are replaced: libcdvd
   (46EB30-46FD58, build.py), sifcmd/sifrpc (48E0F0, 48E7C0; sif.py),
   libgraph's sceGsExecStoreImage (46D9A8; the DMA work). Done for the SDK
   libraries the port keeps running: fileio, libmc, tty, deci2, libmpeg.
"""

import re

# Every changed file gets the header, ahead of its first line; #line keeps
# the original's line numbers
_INCLUDE = (r"\A", '#include "grx_memory.h"\n#line 1\n')


def _alias(text, bits="0x20000000", name="GRX_UNCACHED"):
    """One alias site, by its exact text, which must end in the alias bits"""
    assert text.endswith(bits), text
    return (re.escape(text), text[: -len(bits)].replace("\\", "\\\\") + name)


def _alias_acc(text):
    return _alias(text, "0x30000000", "GRX_UNCACHED_ACC")


def _lit(old, new):
    """One literal site, by its exact text"""
    return (re.escape(old), new.replace("\\", "\\\\"))


_FIXED_RAM = {
    # SysMain: the arena, and the battle and face heaps end to end
    "src/main/4000C0.c": [
        _lit("MemInit(7, 0x760000, 0x1FF0000);",
             "MemInit(7, (s32 *)grx_ee_area(GRX_AREA_HEAP7), (s32)grx_ee_area_end(GRX_AREA_HEAP7));"),
        _lit("MemHeapCreateAt(0x200000, 0x100000);",
             "MemHeapCreateAt((void *)grx_ee_area(GRX_AREA_BATTLE_HEAP), 0x100000);"),
        _lit("MemHeapCreateAt(0x300000, 0x100000);",
             "MemHeapCreateAt((void *)grx_ee_area(GRX_AREA_FACE_HEAP), 0x100000);"),
    ],
    # SysIniLoad: GRX.INI is read to 0x100000 and parsed there
    "src/main/40D860.c": [
        _lit("FileRead(sysIniFileName, 0x100000);",
             "FileRead(sysIniFileName, (s32)grx_ee_area(GRX_AREA_INI_BUF));"),
        _lit("p = (char *)0x100000;", "p = (char *)grx_ee_area(GRX_AREA_INI_BUF);"),
    ],
    # SysMfifoStart: the MFIFO ring (1 MB at 0x100000, aligned to its size)
    "src/main/42FFC8.c": [
        _lit("GsMfifoInit(0x100000, 0x100000);",
             "GsMfifoInit((s32)grx_ee_area(GRX_AREA_MFIFO_RING), 0x100000);"),
    ],
    # The debug memory view: its default base and its fallback address
    "src/main/420808.c": [_lit("dbgMemView.base = 0x770000;", "dbgMemView.base = (s32)grx_ee_ram(0x770000);")],
    "src/main/42B440.c": [_lit("    addr = 0x1000000;\n", "    addr = (s32)grx_ee_ram(0x1000000);\n")],
}

# The game's own alias sites (60 in 23 functions, hardware.md (c)), and
# sceMpegGetPicture, which the survey counts as the game's
_ALIAS_GAME = {
    # graphics
    "src/main/433B10.c": [_alias("MemMove128((void *)((u32)e | 0x20000000")],
    "src/main/433C18.c": [
        _alias("MemMove128((void *)(buf1 | 0x20000000"),
        _alias("pk = (u32 *)(buf2 | 0x20000000"),
        _alias("GphSkinPackNormals((void *)((buf2 + 0x10) | 0x20000000"),
    ],
    "src/main/434280.c": [_alias("p = (Pkt *) (obj->gsPkt | 0x20000000")],
    "src/main/43A240.c": [_alias("MemMove128((void *)((u32)s | 0x20000000")],
    "src/main/4563A0.c": [_alias("*(volatile u32 *)0x70000014 = (u32)gphClipPacketBuf | 0x20000000")],
    "port/game/main/439058.c": [
        _alias("#define UNCACHED(p) ((u8 *)((u32)(p) | 0x20000000"),
        _alias_acc("#define UNCACHED_ACC(p) ((u8 *)((u32)(p) | 0x30000000"),
    ],
    "port/game/main/43CE68.c": [_alias("u = (u8 *)(pkt | 0x20000000")],
    # sound
    "src/main/461B80.c": [
        _alias("hdr = (Hdr *)((u32)sndBankHdr | 0x20000000"),
        _alias("idx = (s32 *)((u32)sndBankHdrIds | 0x20000000"),
        _alias("sub = (SubEnt *)((u32)(sndBankHdrIds + 0x7F0) | 0x20000000"),
    ],
    "port/game/main/460670.c": [_alias("buf = (s8 *)(((sndFsDirBuf + sndFsDirBufOfs + 0x7F) & 0x7FFFF80) | 0x20000000")],
    # movies
    "src/main/464E50.c": [_alias("voBufCreate(&movVoBuf, (movVoBufData & 0x0FFFFFFF) | 0x20000000")],
    "src/main/465148.c": [_alias("voBufCreate(&movVoBuf, (movVoBufData & 0x0FFFFFFF) | 0x20000000")],
    "src/main/4654D0.c": [_alias("voBufCreate(&movVoBuf, (movVoBufData & 0x0FFFFFFF) | 0x20000000")],
    "src/main/4658A8.c": [
        _alias("copy2area((a & 0x0FFFFFFF) | 0x20000000"),
        _alias(", (c & 0x0FFFFFFF) | 0x20000000"),
    ],
    "src/main/465E78.c": [
        _alias("videoDecCopy2area((a & 0x0FFFFFFF) | 0x20000000"),
        _alias(", (c & 0x0FFFFFFF) | 0x20000000"),
    ],
    "src/main/466458.c": [_alias("p->tag = (void *)((tags & 0x0FFFFFFF) | 0x20000000")],
    "src/main/467FD8.c": [_alias("MovGsGifTag(pkt | 0x20000000")],
    # both sites: the packet's start and, later, the offset from it
    # movPktFadeAlpha and the other movPkt* are word offsets into the
    # movie packets, and their 0x08000000 is the uncached alias in words
    # (MovVblankHandler, MovSkip*: p->v[i] + movPktFadeAlpha; the list and
    # ipart vblank handlers, 46B730 and 46C170: b->v[h % 2] + idx). In
    # 468748/469530 both forms, `(q - base) | ...` and `((q - base) + 4) | ...`
    "src/main/468278.c": [_alias("(buf & 0x0FFFFFFF) | 0x20000000"),
                          _alias(">> 2) | 0x08000000", "0x08000000", "GRX_UNCACHED_WORDS")],
    "src/main/468748.c": [_alias("#define UNCACHED(x) (((u32)(x) & 0x0FFFFFFF) | 0x20000000"),
                          _alias("(q - (u32 *)UNCACHED(base)) | 0x08000000", "0x08000000", "GRX_UNCACHED_WORDS"),
                          _alias("(q - (u32 *)UNCACHED(base)) + 4) | 0x08000000", "0x08000000",
                                 "GRX_UNCACHED_WORDS")],
    "src/main/469530.c": [_alias("#define UNCACHED(x) (((u32)(x) & 0x0FFFFFFF) | 0x20000000"),
                          _alias("((q - (u32 *)UNCACHED(base))) | 0x08000000", "0x08000000", "GRX_UNCACHED_WORDS"),
                          _alias("(q - (u32 *)UNCACHED(base)) + 4) | 0x08000000", "0x08000000",
                                 "GRX_UNCACHED_WORDS")],
    "src/main/46A8D8.c": [_alias("MovGsGifTag(new_var | 0x20000000")],
    "src/main/46B730.c": [_alias("#define UNCACHED(x) (((u32)(x) & 0x0FFFFFFF) | 0x20000000")],
    "src/main/46C170.c": [_alias("#define UNCACHED(x) (((u32)(x) & 0x0FFFFFFF) | 0x20000000")],
    "src/main/4726D0.c": [_alias("arg1 = (arg1 & 0x0FFFFFFF) | 0x20000000")],
}

# The SDK libraries the port keeps (their RPC receive buffers and the like)
_FS_RECV = "*(s32 *)((u32)_sceFsRecvBuf | 0x20000000"
_ALIAS_SDK = {
    # libmc
    "src/main/470480.c": [_alias("p = (Xfer *)(arg0 | 0x20000000")],
    "src/main/470900.c": [_alias("temp_a0 = arg0 | 0x20000000")],
    "src/main/470C38.c": [_alias("(char *)((u32)_sceMcCurDirBuf | 0x20000000")],  # 3 sites
    # libmpeg
    "src/main/472718.c": [_alias("c->fD8 = (arg1 & 0x0FFFFFFF) | 0x20000000")],
    "src/main/472760.c": [_alias("arg1 = (arg1 & 0x0FFFFFFF) | 0x20000000")],
    "src/main/4731C8.c": [_alias("tag = (Tag *)(((u32)_sceMpegIpuToTags & 0x0FFFFFFF) | 0x20000000")],
    "src/main/473760.c": [
        _alias("#define UNC(x) (((u32)(x) & 0x0FFFFFFF) | 0x20000000"),
        _alias("*a3 = ym | 0x20000000"),
    ],
    "src/main/473B70.c": [_alias("p = (u64 *)((_sceMpegRefDmaTags & 0x0FFFFFFF) | 0x20000000")],
    # tty
    "src/main/48D420.c": [_alias("buf = (u8 *)((u32)_sceTtySendBuf | 0x20000000")],
    "src/main/48D640.c": [
        _alias("_sceTtyState.rbuf = (void *)((u32)_sceTtyRecvBuf | 0x20000000"),
        _alias("h = (TtyHdr *)((u32)_sceTtySendBuf | 0x20000000"),
    ],
    # fileio
    "src/main/48F878.c": [_alias("#define UNCACHED(p) ((void *)((u32)(p) | 0x20000000")],
    "src/main/48FD88.c": [_alias("*(B4 *)((u32)_sceFsRecvBuf | 0x20000000")],
    "src/main/4908C8.c": [_alias(_FS_RECV), _alias("    addr |= 0x20000000")],
    "src/main/490B88.c": [
        _alias(_FS_RECV),
        _alias("*(s32 *)arg2 = *(s32 *)((u32)_sceFsRetBufData | 0x20000000"),
        _alias("*(s64 *)arg2 = *(s64 *)((u32)_sceFsRetBufData | 0x20000000"),
    ],
    "src/main/491A30.c": [_alias("r = *(s32 *)((u32)recv | 0x20000000")],
    "src/main/492310.c": [_alias("r = *((s32 *) (((u32) _sceFsRecvBuf) | 0x20000000")],
    # deci2
    "src/main/493E28.c": [_alias("buf[3] = (u32)&_sceDeci2Work | 0x20000000")],
}
# fileio's calls that read their RPC result the same way
for _a in ("490018", "4902A0", "490420", "490658", "490ED8", "4910B8", "491280", "491450", "4916C0",
           "4918C8", "491B88", "491D28", "491F68", "492178", "4925A0", "4927D8", "492A10", "492BF0"):
    _ALIAS_SDK[f"src/main/{_a}.c"] = [_alias(_FS_RECV)]

FIXUPS = {}
for _table in (_FIXED_RAM, _ALIAS_GAME, _ALIAS_SDK):
    for _rel, _subs in _table.items():
        assert _rel not in FIXUPS, _rel
        FIXUPS[_rel] = [_INCLUDE] + _subs
