#ifndef MAIN_N06_H
#define MAIN_N06_H

/* Main executable 0x0043C4E0-0x00441F78: the prim list and its 2D/3D line,
 * polygon and textured-prim packets, the water-ripple surfaces, the memory
 * heaps, and the BGM / sound-effect front end (the slot and channel types
 * themselves are in snd.h, the texture table in gph.h). Field names come
 * from docs/notes/names_observations.md (n06), the symbol files' comments and
 * the code. A comment "X in 4xxxxx" records a file whose local view used a
 * different type at that offset. */

#include "common.h"
#include "vu.h"   /* VuVec3, VuVec4 */
#include "snd.h"

/* -------------------------------------------------------------- prim list */

/* One packet of the prim list (GphPrimListAdd 43C5D0, GphPrimListDraw 43C548). */
typedef struct GphPrimEntry {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ u16 texId;        /* bound before the packet is sent, 0 = none (s16 in 43C518/43C5D0) */
    /* 0x4 */ s32 pkt;          /* GIF packet (GsGifPacketSend) */
} GphPrimEntry; /* size = 0x8 */

/* Prim list (gphPrimList; GphPrimListSelect 43C5B8). GphPrimListSetTexture
 * sets entries[count].texId, the texture of the next packet added. */
typedef struct GphPrimList {
    /* 0x0 */ s32 count;
    /* 0x4 */ GphPrimEntry entries[1024];
} GphPrimList; /* size = 0x2004 */

/* Head of the packets the line and textured-prim builders make
 * (GphLine2DDraw 43C6A8, GphTexPrim2DBuild 43DDA8, GphTexPrim3DBuild 43DFB0):
 * DMA tag, VIF DIRECT, then two GIF tags. */
typedef struct GphPrimPkt {
    /* 0x00 */ u64 dmaTag;      /* 0x10000000 | qwc */
    /* 0x08 */ u64 unk8;
    /* 0x10 */ u32 vif0;        /* VIF NOPs */
    /* 0x14 */ u32 vif1;
    /* 0x18 */ u32 vif2;
    /* 0x1C */ u32 vifDirect;   /* 0x50000000 | qwc */
    /* 0x20 */ u64 gifTag;
    /* 0x28 */ u64 gifRegs;
    /* 0x30 */ u64 reg0;        /* PRIM for lines */
    /* 0x38 */ u64 reg1;        /* RGBAQ for lines */
    /* 0x40 */ u64 gifTag2;     /* EOP | vertex count */
    /* 0x48 */ u64 gifRegs2;
    /* 0x50 */ u64 data[1];     /* the vertices */
} GphPrimPkt;

/* ---------------------------------------------------------------- ripples */

/* Water-ripple surface (GphRippleCreate 43FA78; work size GphRippleWorkSize
 * 43FA58 = 0x24 + two w * (h + 4) height fields). */
typedef struct GphRipple {
    /* 0x00 */ s32 *src;        /* image the ripple displaces (GphRippleApply32 440118) */
    /* 0x04 */ s32 *dst;
    /* 0x08 */ s32 *bufBase[2]; /* height fields with two margin rows above and below */
    /* 0x10 */ s32 *buf[2];     /* = bufBase[i] + 2 rows */
    /* 0x18 */ s32 w;
    /* 0x1C */ s32 h;           /* u32 in 43FCE8 */
    /* 0x20 */ s32 cur;         /* buf[] index of the current field */
    /* 0x24 */ s32 data[1];
} GphRipple;

/* ----------------------------------------------------------------- memory */

/* Heap block header (MemAlloc 440590, MemFree 440668). Free blocks form a
 * circular list from the heap's head; an allocation's user pointer follows
 * self/guard. */
typedef struct MemBlock {
    /* 0x00 */ struct MemBlock *next;
    /* 0x04 */ s32 heapId;      /* owner (MemFree looks the heap up by it) */
    /* 0x08 */ s32 size;        /* bytes including the header; u32 in 4404E0 */
    /* 0x0C */ struct MemBlock *self; /* allocated: the block, read back by MemFree */
    /* 0x10 */ u32 guard[4];    /* 0xC0C0C0C0 */
} MemBlock; /* size = 0x20 */

/* Heap (MemHeapCreate 440268, MemHeapCreateAt 440320). */
typedef struct MemHeap {
    /* 0x00 */ s32 id;
    /* 0x04 */ u16 inUse;       /* entry taken (MemHeapEntryAlloc); s16 in 440268/440320 */
    /* 0x06 */ s16 dirty;       /* 1 after MemAlloc, 0 after create/reset */
    /* 0x08 */ u32 base;        /* first block (MemBlock *): 440268/440320 only match with an
                                 * integer store, 4403B0/440590 cast it */
    /* 0x0C */ u32 end;
    /* 0x10 */ s32 size;
    /* 0x14 */ MemBlock *head;  /* free list */
    /* 0x18 */ u8 pad18[8];
} MemHeap; /* size = 0x20 */

/* Heap table (memHeapTable, MemInit 4401F8 clears (count + 1) * 0x20 bytes). */
typedef struct MemHeapTable {
    /* 0x00 */ s32 count;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ MemHeap heaps[1]; /* count entries */
} MemHeapTable;

/* ------------------------------------------------------------------ sound */

/* SndPlayAtPos argument block (SndSeUpdate 4419B0). */
typedef struct SndPlayPosReq {
    /* 0x0 */ s32 handle;
    /* 0x4 */ s32 index;
    /* 0x8 */ f32 *pos;
} SndPlayPosReq; /* size = 0xC */

/* SE RPC argument blocks and the SE bank loader (sndSeWork; SndSeUpdate
 * 4419B0, SndSeBankLoadStart 4413C8, SndSeBankLoadUpdate 441430). */
typedef struct SndSeWork {
    /* 0x00 */ SndPrepareReq prepare; /* SE prepare, also the bank's (handle, index, fd, sectors) */
    /* 0x10 */ SndPlayReq play;
    /* 0x24 */ SndPlayPosReq playPos;
    /* 0x30 */ SndStatus status;
    /* 0x40 */ s32 result;
    /* 0x44 */ u8 pad44[8];
    /* 0x4C */ s32 loadState;   /* bank load step, -1 when done (sndSeLoadState) */
    /* 0x50 */ s32 bankHandle;
    /* 0x54 */ s32 bankIndex;
    /* 0x58 */ s32 fd;          /* SndStrmOpen */
    /* 0x5C */ s32 sectors;     /* file size in 2048-byte sectors */
    /* 0x60 */ s32 ret;         /* last call's result */
    /* 0x64 */ s32 unk64;
    /* 0x68 */ char name[32];
} SndSeWork; /* size = 0x88 */

/* ---------------------------------------------------------------- globals */

extern s16 gphSphereStacks;     /* 0x004A94A0; u16 in 43D550 */
extern s32 gphPrimColor;        /* 0x004A94A4 */
extern s16 gphSphereSlices;     /* 0x004A94A8; u16 in 43D550 */
extern GphPrimList *gphPrimList; /* 0x004B7084 */

extern MemHeapTable *memHeapTable; /* 0x004B7094 */
extern MemHeap *memCurHeap;     /* 0x004B7098 */
extern s32 memNextHeapId;       /* 0x0049AA44 */
extern u32 memArenaCur;         /* 0x004B708C, next free address; s32 * in 4401F8 (now cast) */
extern u32 memArenaEnd;         /* 0x004B7090 */

extern s32 sndBgmCurChannel;    /* 0x004B709C */
extern s32 sndBgmEnabled;       /* 0x004B70A0 */
extern s32 sndSeActive;         /* 0x004B70A8 */
extern s32 sndSeRequestEnabled; /* 0x004B70AC */
extern SndSeWork sndSeWork;     /* 0x004F2D80 */

#endif
