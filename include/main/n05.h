#ifndef MAIN_N05_H
#define MAIN_N05_H

/* Main executable 0x00436CE8-0x0043C4E0: model data accessors (parts,
 * attributes, motion, part positions), the model instance pool, vertex
 * deformers, planar shadows, projected textures and environment maps.
 * Models, parts and GA objects themselves are in gph.h; field names come
 * from docs/notes/names_observations.md (n05) and the symbol files' comments. */

#include "common.h"
#include "vu.h"
#include "gph.h"
#include "chr.h"

/* ----------------------------------------------------------- model data */

/* ChrModelData is in chr.h. */

/* ------------------------------------------------------ instance pool */

/* Bump allocator the model instances come from (ChrModelPoolInit 438128,
 * ChrModelPoolAlloc 438160: 16-byte aligned). */
typedef struct ChrModelPool {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 start;
    /* 0x0C */ s32 cur;
    /* 0x10 */ s32 end;
} ChrModelPool; /* size = 0x14 */

extern ChrModelPool *chrModelPool;      /* 0x004B6304 (ChrModelPoolSet/Get pass it as s32) */

/* ChrModelCreate 4381D8 / ChrModelCreateParts 438378 work */
extern s32 chrModelCreateUnused0;       /* 0x0049A95C */
extern s32 chrModelCreateUnused1;       /* 0x0049A960 */
extern s32 chrModelCreateUnused2;       /* 0x0049A964 */
extern GphPart *chrModelCreatePartPtr;  /* 0x004B6308 */
extern GphModel *chrModelCreated;       /* 0x004B630C */
extern GphGaObject *chrModelCreateGaObjPtr; /* 0x004B6310 */
extern f32 *chrModelCreateMtxPtr;       /* 0x004B6314, next 0x40-byte part matrix; f32 * like
                                         * GphModel.mtx/restMtx (4381D8's scheduling needs the
                                         * same alias set) */
extern s32 chrModelCreatePktSize;       /* 0x004B6318 */
extern s32 chrModelCreateHasDeform;     /* 0x004B631C */

/* GsAdReg / GsAdPkt are in gph.h. */

/* ------------------------------------------------------- planar shadows */

extern FVECTOR gphShadowColor;          /* 0x0049B280, default -255, -255, -255, 255 (43A370) */
extern GsAdPkt gphShadowZbufPkt;        /* 0x004B6340, 1 register: ZBUF_1 */
extern GsAdPkt gphShadowGsPkt;          /* 0x004B6380, 4 registers: FBA_1, ALPHA_1, TEST_1, PRMODE */

/* ---------------------------------------------------- projected textures */

/* Projector (GphProjTexSetParam 43A948, GphProjTexSetup 43A990). */
typedef struct GphProjTexParam {
    /* 0x00 */ FMATRIX mtx[2];  /* GphProjTexCalcMtx */
    /* 0x80 */ FVECTOR pos;     /* origin + dir * radius.z / 2 */
    /* 0x90 */ FVECTOR radius;
    /* 0xA0 */ FVECTOR dir;
    /* 0xB0 */ u8 padB0[0x10];
} GphProjTexParam; /* size = 0xC0 */

/* Entry (0x18) added by GphProjTexAddEntry 43AA10, drawn by GphProjTexDraw 43ADC8. */
typedef struct GphProjTexEntry {
    /* 0x00 */ s32 type;        /* 0 skip, 1 GA object */
    /* 0x04 */ s32 layer;       /* gaObj->clip, copied back to the GA object by 43ADC8 */
    /* 0x08 */ GphGaObject *gaObj; /* packet copy of the GA object */
    /* 0x0C */ s32 padC;
    /* 0x10 */ s32 pad10;
    /* 0x14 */ s32 mtx;         /* matrix packet */
} GphProjTexEntry; /* size = 0x18 */

typedef struct GphProjTexList {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 pad4[3];
    /* 0x10 */ GphProjTexEntry entries[1]; /* GphProjTexCalcWorkSize 43A910: 0x10 + 0x18 * n */
} GphProjTexList;

extern GphProjTexList *gphProjTexList;  /* 0x004A949C (GphProjTexSetList passes it as s32) */
extern GsAdPkt gphProjTexGsPkt;         /* 0x0049A970 */
extern GphProjTexParam gphProjTexParam; /* 0x004F2A40 */

/* ---------------------------------------------------- environment maps */

/* Environment map slot (0x180), GphEnvMapSetTex 43BA60. */
typedef struct GphEnvMap {
    /* 0x000 */ u8 texPkt[0xEC]; /* texture load packet (GsTexLoadPacket), uploaded by 43BCD8 */
    /* 0x0EC */ s32 texW;       /* inside the packet; w * h is added to the upload stats */
    /* 0x0F0 */ s32 texH;
    /* 0x0F4 */ u8 padF4[0x4C];
    /* 0x140 */ FVECTOR color;  /* 0, 0, 0, 32 (43BA60); gphEnvMapColor (0x004B65C0) labels slot 0's */
    /* 0x150 */ s32 tex;        /* TXD */
    /* 0x154 */ u8 pad154[0x2C];
} GphEnvMap; /* size = 0x180 */

#define GPH_ENVMAP_COUNT 8

extern GphEnvMap gphEnvMapTable[GPH_ENVMAP_COUNT]; /* 0x004B6480 */
extern GsAdPkt gphEnvMapGsPkt;          /* 0x0049B390, 4 registers (room for 5): PRMODE, TEX1_1, TEST_1, ALPHA_1 */

#endif
