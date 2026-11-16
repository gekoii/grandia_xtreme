#ifndef MAIN_N04_H
#define MAIN_N04_H

/* Main renderer (0x00431F38-0x00436CE8): the matrix stack and pool, motion
 * track records and the current draw list. Lights, models, parts, GA
 * objects, motion cursors and draw lists themselves are in gph.h. */

#include "common.h"
#include "vu.h"
#include "gph.h"

/* ------------------------------------------------------------ matrices */

/* Matrix stack control block (vuMtxStackBuf 0x004AC2C8 is the one the game
 * uses; VuMtxStackSet 4323C0 points vuMtxStack at it). VuMtxStackInit
 * 4323D8, Push 432400.., Pop/PopN 4324D0, GetTop 432548. */
typedef struct VuMtxStack {
    /* 0x00 */ FMATRIX *base;
    /* 0x04 */ FMATRIX *cur;    /* top of the stack (VuMtxStackGetTop) */
    /* 0x08 */ FMATRIX *end;    /* base + count */
    /* 0x0C */ s32 count;
} VuMtxStack; /* size = 0x10 */

/* Per-frame matrix pool: VuMtxPoolInit 432568 (0x1000 matrices from
 * 41D1B8), Alloc 432580, AllocN 432598, Reset 4325C8. */
typedef struct VuMtxPool {
    /* 0x00 */ FMATRIX *base;
    /* 0x04 */ FMATRIX *cur;    /* next free matrix (also labelled vuMtxPoolCur) */
    /* 0x08 */ s32 count;
} VuMtxPool; /* size = 0xC */

extern VuMtxStack *vuMtxStack;  /* 0x004A9498 */
extern VuMtxPool vuMtxPool;     /* 0x004F2A00 */
extern FMATRIX *vuMtxPoolCur;   /* 0x004F2A04 = vuMtxPool.cur, its own label (VuMtxPoolGetCur 4325B8) */

/* -------------------------------------------------------------- motion */

/* Per-part record of a motion (GphMotCursor.track points at it): this
 * header, then one key-track pointer (GphMotKeys *) per bit set in flags,
 * in bit order: 1 position, 2 rotation, 4 scale, 8 quaternion rotation,
 * 0x10 vertex morph, 0x20 normal morph, 0x40 alpha, 0x80 UV offset
 * (GphMotPartCalcMatrixEx 435E90, GphPartMorph 4356E8).
 * GphMotCursorNextPart 434FB8 skips count + 1 words to the next part. */
typedef struct GphMotTrackHdr {
    /* 0x00 */ u16 count;       /* key-track pointers that follow */
    /* 0x02 */ u16 flags;
} GphMotTrackHdr; /* size = 0x4, followed by the key-track pointers */

/* Key track. count == 1 is a constant (values[0]); otherwise
 * frameIndex[(s32)frame] is the key k with frames[k] <= frame, and the
 * value is interpolated toward key k + 1. values holds one vertex set
 * (s128) per key in GphMorphPositions 4357A0, normal sets (s64) in
 * GphMorphNormals 435890, s8 alphas in GphMotKeyAlpha 436058 and
 * {f32 u, v} pairs in GphMotKeyUvOffset 4361A8. */
typedef struct GphMotKeys {
    /* 0x00 */ s32 count;
    /* 0x04 */ void *values;
    /* 0x08 */ u16 *frameIndex;
    /* 0x0C */ u16 frames[1];   /* count entries */
} GphMotKeys;

/* GphModelMotion is in gph.h. */

/* ----------------------------------------------------------- draw list */

/* The current draw list lives in scratchpad 0x70002708 (GphDrawListSet
 * 436390, GphDrawListModuleInit 436308): gph.h GphRenderCtx.drawList. */
#define gphDrawListCur (gphRenderCtx->drawList)

#endif
