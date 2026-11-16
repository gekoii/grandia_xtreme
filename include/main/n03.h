#ifndef MAIN_N03_H
#define MAIN_N03_H

/* Main executable 0x0042C330-0x00431B10: memory card save support, the
 * debug text overlay, system parameters, compressed data and disc reads,
 * views, boot/frame control, GS packet helpers and the double-buffered
 * packet area. Viewports (GphView) are in gph.h, the camera in camera.h,
 * pads in pad.h. Field names come from docs/notes/names_observations.md (n03),
 * the symbol files' comments and the code. */

#include "common.h"
#include "sdk/libmc.h"
#include "mc.h"
#include "gph.h"   /* gsDispEnv, gsDispFrameAddr */

/* ---------------------------------------------------------- memory card */

/* The save-file table, card state and slot table are in mc.h. */

/* Directory listing the card driver fills (mcDirListPtr -> mcDirListBuf). */
typedef struct McDirList {
    /* 0x00 */ s32 max;         /* 0x10 */
    /* 0x04 */ s32 count;
    /* 0x08 */ u8 pad8[0x38];
    /* 0x40 */ sceMcTblGetDir entries[1]; /* count entries */
} McDirList;

extern McDirList *mcDirListPtr;         /* 0x0049A8C4 */

extern u8 mcCrcTable[0x100];            /* 0x004ACDC0, CRC-32 table truncated to bytes (McCrcTableInit 42D228) */
extern s32 mcAccessMode;                /* 0x0049A8C0, Save/Load/Boot */
extern char mcSaveNamePrefix[];         /* "save" */

/* MC debug menu. */
struct Pad;
extern s32 mcDbgPage;                   /* 0x004B05C0, 0 top, 1 command */
extern struct Pad *mcDbgPad;            /* 0x004B05CC = &padPort0[0] */
extern struct Pad *mcDbgPadTrig;        /* 0x004B05D0 = &padPort0[1] (not edge bits) */

/* ---------------------------------------------------------- debug text */

/* Text command buffer: characters, and words > 0xFFFF that set colour
 * (0x80000000 | n), char width/height (0x40000000/0x20000000) or x/y
 * (0x10000000/0x08000000). DbgPrintf/DbgPuts append, DbgTextDrawChars
 * 42E4C0 draws, DbgTextClear clears all 0x4004 bytes. */
typedef struct DbgTextBuf {
    /* 0x0000 */ u32 buf[0x1000];
    /* 0x4000 */ u32 count;
} DbgTextBuf; /* size 0x4004 */

extern DbgTextBuf dbgTextBuf;           /* 0x004B0FC0 */
extern u32 dbgCharW;                    /* 0x004B4FC4, character width x16 */
extern u32 dbgCharH;                    /* 0x004B4FC8 */
extern s32 dbgTextOfsX;                 /* 0x004B4FCC */
extern s32 dbgTextOfsY;                 /* 0x004B4FD0 */
extern u64 dbgTextColors[16];           /* 0x004B4FD8, RGBAQ */
extern u8 dbgTextGsState[];             /* 0x004B5080, GsRenderStatePacketSet */
extern u8 dbgFontTexPkt[0x140];         /* 0x004B5140, GsTexPacketSet */

/* -------------------------------------------------------- system params */

/* 12-byte state set from system init (SysParamInit 42E938, SysParamSet
 * 42E998, SysParamSetMode 42E988), never read. */
typedef struct SysParams {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 mode;         /* sysParamMode */
} SysParams; /* size 0xC */

extern SysParams sysParams;             /* 0x004B5280 */
extern s32 sysParamMode;                /* 0x004B5288 = sysParams.mode */

/* ------------------------------------------------------------ disc files */

/* Compressed file header, xor-coded with its first word (FileCmpReadHeader
 * 42E9E0 decodes it). */
typedef struct FileCmpHeader {
    /* 0x0 */ s32 key;
    /* 0x4 */ s32 magic;        /* 0x15246468 */
    /* 0x8 */ s32 size;         /* unpacked size (FileCmpGetSize) */
    /* 0xC */ s32 unkC;         /* FileLzssDecode's input length */
} FileCmpHeader; /* size 0x10 */

/* SndFsFind output. */
typedef struct FileFindInfo {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 size;         /* bytes */
    /* 0x8 */ s32 pad8[2];
} FileFindInfo; /* size 0x10 */

/* Disc read work (fileReadWork 0x004F2B80): FileReadStart 42ED18 sets it
 * up, FileReadUpdate 42EDE0 steps open / read start / get ready / fetch /
 * close, logging every result. */
typedef struct FileReadWork {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 state;      /* -1 idle, 0-5 step (fileReadState) */
    /* 0x008 */ s32 fd;
    /* 0x00C */ u8 *buf;        /* destination, advanced per chunk */
    /* 0x010 */ s32 remain;     /* sectors left */
    /* 0x014 */ s32 unk14;
    /* 0x018 */ s32 sectors;    /* file size in 2048-byte sectors */
    /* 0x01C */ s32 unk1C;
    /* 0x020 */ s32 chunk;      /* sectors ready (SndStrmGetReady) */
    /* 0x024 */ s32 retry[6];   /* failures per step */
    /* 0x03C */ s32 unk3C;
    /* 0x040 */ s32 total[6];   /* retries summed over every read */
    /* 0x058 */ u8 pad58[8];
    /* 0x060 */ s32 log[6][16]; /* results per step (FileLogResult) */
    /* 0x1E0 */ char name[32];
} FileReadWork; /* size 0x200 */

extern FileReadWork fileReadWork;
extern s32 fileReadState;               /* 0x004F2B84 = fileReadWork.state */

/* ------------------------------------------------------ system / GS */

extern char sysBootPath[0x40];          /* 0x004B5F30, argv0 */
extern char sysLibVersion[0x100];       /* 0x004B5F70, "GRX Library Jul 30 2002" */
extern u8 gphViewGsState[0x90];         /* 0x004B5CC0, GS state of the views */
/* gsDispFrameAddr and gsDispEnv are in gph.h. */
extern u8 sysClearDrawEnv[0x80];        /* 0x004B5D80 */
extern u8 sysClearGsState[0x90];        /* 0x004B5E00 */

/* Full-screen sprite packet SysClearScreen sends (SysSetClearColor 430028):
 * DMA tag, VIF, GIF tag (PRIM, RGBAQ, XYZ2, XYZ2), then the registers. */
typedef struct SysClearPkt {
    /* 0x00 */ u64 dmaTag;
    /* 0x08 */ u64 dmaTagHi;
    /* 0x10 */ s32 vif0;
    /* 0x14 */ s32 vif1;
    /* 0x18 */ s32 vif2;
    /* 0x1C */ s32 direct;      /* VIF DIRECT */
    /* 0x20 */ u64 gifTag;
    /* 0x28 */ u64 gifRegs;
    /* 0x30 */ u64 prim;        /* 6 = sprite */
    /* 0x38 */ u64 rgbaq;       /* clear colour, Q = 1.0 */
    /* 0x40 */ u64 xyz0;
    /* 0x48 */ u64 xyz1;
    /* 0x50 */ u8 pad50[0x50];
} SysClearPkt; /* size 0xA0 (cleared by SysSetClearColor) */

extern SysClearPkt sysClearPkt;         /* 0x004B5E90 */

/* Double-buffered packet area (GsPacketBufInit 430F70, GsPacketBufSwap
 * 430FC8, GsPacketAlloc* 431038-4310C8). */
typedef struct GsPacketBuf {
    /* 0x00 */ u32 size;        /* bytes per buffer (half the area) */
    /* 0x04 */ s32 index;       /* buffer in use, toggled by the swap */
    /* 0x08 */ u32 buf0;        /* 16-aligned start */
    /* 0x0C */ u32 buf1;        /* buf0 + size */
    /* 0x10 */ u32 start;       /* current buffer */
    /* 0x14 */ u32 cur;         /* next free byte */
} GsPacketBuf; /* size 0x18 */

extern GsPacketBuf *gsPacketBuf;        /* 0x0049A958 */

/* The scratchpad render context (GphRenderCtx) is in gph.h. */

#endif
