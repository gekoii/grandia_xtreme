#ifndef MAIN_N07_H
#define MAIN_N07_H

/* Main 0x00441F78-0x00453E78: frame-buffer effects, chunked file relocation,
 * 2D primitive batches, the particle system's draw context, screen wipes and
 * fades, font images, 2D anim sets and the PAC archives (model, motion and
 * character packs, face animation). Layouts come from the files' own views
 * (docs/notes/names_observations.md, n07). The frame-buffer packet, textures,
 * anim sets, GA objects and particle structs themselves are in gph.h and
 * ptcl.h. */

#include "common.h"
#include "vu.h"
#include "gph.h"
#include "chr.h"

/* The GS frame buffer addresses (gsDrawFrameAddr, gsZBufAddr) are in gph.h. */

/* ---------------------------------------------- frame-buffer effects */

/* Area of a view as GphViewGetArea 0x0042F6B0 returns it (s32 x 4). */
typedef struct GphViewArea {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 w;
    /* 0xC */ s32 h;
} GphViewArea; /* size = 0x10 */

/* One row of the 9-divide tiles (GphFbEffectInit 441F78, scrolled by
 * GphFbNineDivideScrollV/H 445EE0/4465A0). */
typedef struct GphNineDivRow {
    /* 0x00 */ f32 x[3];
    /* 0x0C */ f32 y[3];
} GphNineDivRow; /* size = 0x18 */

/* Mesh-warp spring grid (GphFbWarpMeshStep 443570, GphFbMeshWarp 4437C0):
 * 9x9 UV offsets, their velocities and the previous step. */
extern f32 gphWarpMeshX[9][9];  /* 0x004B70B8 */
extern f32 gphWarpMeshY[9][9];  /* 0x004B7200 */
extern f32 gphWarpVelX[9][9];   /* 0x004B7348 */
extern f32 gphWarpVelY[9][9];   /* 0x004B7490 */
extern f32 gphWarpPrevX[9][9];  /* 0x004B75D8 */
extern f32 gphWarpPrevY[9][9];  /* 0x004B7720 */
extern GphNineDivRow gphNineDivTilesV[3]; /* 0x004B7868 */
extern GphNineDivRow gphNineDivTilesH[3]; /* 0x004B78B0 */

/* ------------------------------------------------- chunked file data */

/* Chunk list FileChunkParse 4483B0 / FileChunk8List 448608 fill. */
typedef struct FileChunkEntry {
    /* 0x0 */ s32 id;           /* chunk tag */
    /* 0x4 */ s32 *data;
    /* 0x8 */ s32 size;         /* bytes (not set by FileChunk8List) */
} FileChunkEntry; /* size = 0xC */

typedef struct FileChunkTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ FileChunkEntry entries[1]; /* count entries */
} FileChunkTable;

/* --------------------------------------------------- 2D primitive batches */

/* Line of a GphBatchDrawLines 449728 list. */
typedef struct GphBatchLine {
    /* 0x00 */ s32 type;        /* blend state, 0 = end */
    /* 0x04 */ s32 z;
    /* 0x08 */ u8 c0[4];        /* RGBA */
    /* 0x0C */ s16 x0;
    /* 0x0E */ s16 y0;
    /* 0x10 */ u8 c1[4];
    /* 0x14 */ s16 x1;
    /* 0x16 */ s16 y1;
    /* 0x18 */ u8 pad18[8];
} GphBatchLine; /* size = 0x20 */

/* DMA/VIF/GIF header the batch packets start with (the same words as
 * GphFbPacket +0x340). */
typedef struct GphBatchPktHdr {
    /* 0x00 */ u64 dmaTag;
    /* 0x08 */ u64 dmaTagHi;
    /* 0x10 */ u32 vif[4];
    /* 0x20 */ u64 gifTag;
    /* 0x28 */ u64 gifRegs;
    /* 0x30 */ u64 prim;
} GphBatchPktHdr; /* size = 0x38 */

/* Packet templates (gphBatchPackets, GphBatchInit 448798), two of each kind
 * for double buffering. */
typedef struct GphBatchQuadPkt {
    /* 0x00 */ GphBatchPktHdr h;   /* prim 4: triangle strip */
    /* 0x38 */ struct {
                   u64 rgbaq;
                   u64 uv;
                   u64 xyz;
               } v[4];
    /* 0x98 */ u64 end;
} GphBatchQuadPkt; /* size = 0xA0 */

typedef struct GphBatchLinePkt {
    /* 0x00 */ GphBatchPktHdr h;   /* prim 1: line */
    /* 0x38 */ struct {
                   u64 rgbaq;
                   u64 xyz;
               } v[2];
    /* 0x58 */ u64 end;
} GphBatchLinePkt; /* size = 0x60 */

typedef struct GphBatchSpritePkt {
    /* 0x00 */ GphBatchPktHdr h;   /* prim 6: sprite */
    /* 0x38 */ u64 rgbaq;
    /* 0x40 */ struct {
                   u64 uv;
                   u64 xyz;
               } v[2];
    /* 0x60 */ u8 pad60[0x20];
} GphBatchSpritePkt; /* size = 0x80 */

typedef struct GphBatchPackets {
    /* 0x000 */ GphBatchQuadPkt quad[2];
    /* 0x140 */ GphBatchLinePkt line[2];
    /* 0x200 */ GphBatchSpritePkt sprite[2];
} GphBatchPackets; /* size = 0x300 */

/* GS render state packet (GsRenderStatePacketSet), one per blend type. */
typedef struct GphBatchBlendState {
    /* 0x00 */ u8 pkt[0x90];
} GphBatchBlendState; /* size = 0x90 */

/* What each draw works on: the packets and the blend states copied to
 * scratchpad 0x70003180 (0xE80 bytes, MemMove128). */
typedef struct GphBatchScratch {
    /* 0x000 */ GphBatchPackets pkts;
    /* 0x300 */ GphBatchBlendState states[20]; /* primitive type t uses states[t - 1] */
    /* 0xE40 */ FVECTOR projOut[4];  /* GphBatchDrawQuads3D 449B48 projected corners */
} GphBatchScratch; /* size = 0xE80 */

extern GphBatchPackets gphBatchPackets;        /* 0x004D9980 */
extern GphBatchBlendState gphBatchBlendStates[]; /* 0x004D9C80, right after the packets */
extern s64 gphBatchOffsetX;                    /* 0x004DA800, GphBatchSetOffset; u64 in 449490 */
extern s64 gphBatchOffsetY;                    /* 0x004DA808 */

/* ----------------------------------------------------------- particles */

extern FVECTOR ptclDefaultColor;  /* 0x0049AB90, ptclDrawCtx.ambient unless PtclDrawStrip gets one */
extern GphGaObject ptclDrawCtx;   /* 0x004DA900, strip draw object (PtclInit 44A1F8, PtclDrawStrip 4505B0) */
extern u32 ptclStripUvs[];        /* 0x004DABC0, VIF header + 31 x 4 UVs */
extern u32 ptclStripColors[];     /* 0x004DADC0, VIF header + 31 x 4 RGBA */

/* --------------------------------------------------------- screen wipes */

/* Wipe state (gphWipe, GphWipeStart 451B90 / Update 451BF0). */
typedef struct GphWipe {
    /* 0x0 */ s8 active;
    /* 0x1 */ s8 type;          /* 0 slide (GphWipeSlide), 2 shutter (GphWipeShutter); 7 types */
    /* 0x2 */ u8 dir;           /* slide: bit 0 picks the gphWipeSlideTable entry */
    /* 0x3 */ s8 mode;          /* bit 0: slide table, shutter direction */
    /* 0x4 */ s8 state;         /* 2 = done (the frame count stops) */
    /* 0x5 */ s8 frame;         /* u8 in 451BF0 */
} GphWipe; /* size = 0x6 */

extern GphWipe gphWipe;                 /* 0x004DB6B0 */
extern GphBatchQuad gphWipeSprites[8];  /* 0x004DB4B0, GphBatchDrawSprites list (0x200 bytes) */

/* ---------------------------------------------------------- screen fades */

/* GphFade is in gph.h. */
extern GphFade gphScreenFade;   /* 0x004DB6C0 */

/* ------------------------------------------------------------ font images */

extern u32 txtFontAlphaClut[2][16]; /* 0x004DB7F0, two 16-colour alpha ramps (TxtFontClutInit) */
extern u8 txtFontImageEnable;       /* 0x004DB8B0; s8 in 452268/452478 (stores only) */

/* ----------------------------------------------------------- PAC archives */

/* Model pack (FilePacModelRelocate 453508): offsets become pointers. */
typedef struct FilePacModelPack {
    /* 0x00 */ u32 modelCount;
    /* 0x04 */ u32 texCount;
    /* 0x08 */ u8 pad8[0x18];
    /* 0x20 */ u32 *models;     /* chunk data (FileChunkRelocate); FilePacGetModel returns +0x10 */
    /* 0x24 */ u32 *textures;   /* texture files (GphTexDataRelocate, GphTexRegister) */
    /* 0x28 */ u8 *modelData;   /* base of models[] offsets */
    /* 0x2C */ u8 *texData;     /* base of textures[] offsets */
} FilePacModelPack; /* size >= 0x30 */

/* Motion pack (FilePacMotionRelocate 4537D0, FilePacGetMotion 453798). */
typedef struct FilePacMotionEntry {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 offset;       /* chunk, relative to data; FilePacGetMotion returns +0x10 */
    /* 0x8 */ s32 unk8;
    /* 0xC */ s32 unkC;
} FilePacMotionEntry; /* size = 0x10 */

typedef struct FilePacMotionPack {
    /* 0x0 */ u32 count;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ FilePacMotionEntry *entries;
    /* 0xC */ s32 data;
} FilePacMotionPack; /* size = 0x10 */

/* The character pack (ChrPack), its binding and the face animation are in
 * chr.h. */

#endif
