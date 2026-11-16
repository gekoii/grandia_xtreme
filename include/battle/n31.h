#ifndef BATTLE_N31_H
#define BATTLE_N31_H

#include "common.h"
#include "vu.h"
#include "gph.h"
#include "battle/camera.h"
#include "battle/misc.h"

/*
 * BATTLE camera modes, magic effect programs, model helpers and the
 * environment (0x006ADD10-0x006B8CB0): what the files there share beyond
 * battle/camera.h, battle/env.h and battle/misc.h.
 */

/* ------------------------------------------------------------------ */
/* Camera mode procs (BtlCamProc*)                                     */
/* ------------------------------------------------------------------ */

/* Mode procs (BtlCamCtrl.proc, chosen by BtlCamSetMode 0x006AE2F8). */
void BtlCamProcOverview(BtlCamCtrl *, BtlCamParam *);         /* 0x006AEB50 */
void BtlCamProcOverviewInit(BtlCamCtrl *, BtlCamParam *);     /* 0x006AEE38 */
void BtlCamProcBossOverview(BtlCamCtrl *, BtlCamParam *);     /* 0x006AEF98 */
void BtlCamProcBossOverviewInit(BtlCamCtrl *, BtlCamParam *); /* 0x006AF190 */
void BtlCamProcUnitFixed(BtlCamCtrl *, BtlCamParam *);        /* 0x006AF298 */
void BtlCamProcUnitFollow(BtlCamCtrl *, BtlCamParam *);       /* 0x006AF300 */
void BtlCamProcUnitAndTarget(BtlCamCtrl *, BtlCamParam *);    /* 0x006AF378 */
void BtlCamProcUnitCloseUp(BtlCamCtrl *, BtlCamParam *);      /* 0x006AF4D0 */
void BtlCamProcFar(BtlCamCtrl *, BtlCamParam *);              /* 0x006AF5E8 */
void BtlCamProcUnitPair(BtlCamCtrl *, BtlCamParam *);         /* 0x006AF628 */
void BtlCamProcTargets(BtlCamCtrl *, BtlCamParam *);          /* 0x006AF700 */
void BtlCamProcBehindUnit(BtlCamCtrl *, BtlCamParam *);       /* 0x006AF9A8 */
void BtlCamProcChaseUnit(BtlCamCtrl *, BtlCamParam *);        /* 0x006AFAC0 */

/* ------------------------------------------------------------------ */
/* Magic effect programs (Mgc*)                                        */
/* ------------------------------------------------------------------ */

/* Stand-in effect program procs used when the program file is missing
 * (MgcLoadFile 0x006B29B8 with flag 0x10); MgcProc type. */
void MgcFallbackMagicStart(MgcWork *); /* 0x006B32C0 */
void MgcFallbackMagicCast(MgcWork *);  /* 0x006B3318 */
void MgcFallbackMagicHit(MgcWork *);   /* 0x006B3360 */
void MgcFallbackSkillStart(MgcWork *); /* 0x006B3400 */
void MgcFallbackSkillHit(MgcWork *);   /* 0x006B3458 */

struct GphModel;        /* gph.h */
struct ChrModelData;    /* chr.h */

/* Effect models (mgcWork.modelData / .models): part visibility back to the
 * model data's (or all shown when mgcWork.unk13 & 6). */
void MgcModelResetPartDisp(struct GphModel *dst, struct ChrModelData *src); /* 0x006B2E78 */

/* ------------------------------------------------------------------ */
/* Environment (Env*): lens flare, stage                               */
/* ------------------------------------------------------------------ */

/* Lens flare sprites (EnvFlareDraw 0x006B7B68 builds two GphBatchQuads on the
 * stack, the second's type 0 ending the list; EnvFlareDrawSprite places one). */
void EnvFlareDrawSprite(s32 id, GphBatchQuad *q, f32 alpha, f32 t, f32 size); /* 0x006B7900 */
struct EnvParam; /* battle/env.h */
void EnvFlareCalcPos(struct EnvParam *e); /* 0x006B7858: envFlarePos from the record's flare angles */

/* ------------------------------------------------------------------ */
/* ENV debug pages (DbgEnvLightPage 0x006B5E58, DbgEnvShadowPage         */
/* 0x006B7368, DbgEnvContrastPage 0x006B75C8)                          */
/* ------------------------------------------------------------------ */

extern char dbgEnvTitleColorStr[]; /* title colour escape */
extern char dbgEnvColor0Str[];     /* back to colour 0 */

/* ------------------------------------------------------------------ */
/* Model helpers (BtlChr*, 0x006B3540-0x006B3E20)                      */
/* ------------------------------------------------------------------ */

/*
 * BtlChrPartsSet* (0x006B36F0, 0x006B3730, 0x006B3788, 0x006B37F0,
 * 0x006B3BA8) loop over GphPart.groupPartCount parts from the one passed;
 * BtlChrTex* (0x006B38A8-0x006B3B48) remap the texture slot (unk3) of each
 * of ChrModelData.gaObjs and compare GphTex.texId.
 */

struct GphTexTable;     /* gph.h */
struct GphTex;

/*
 * gphTexTableBuf as BtlChrTexFindById / BtlChrTexFindSlotById index it:
 * from the table start with the slot stride, so row[i].src is
 * slots[i].src (gph.h GphTexTable; GphTexSlot.src is at +8 of a slot).
 * Indexing GphTexTable.slots[] instead hoists &slots[0].src out of the loop.
 */
typedef struct BtlTexTableRow {
    /* 0x000 */ u8 pad0[0x48];
    /* 0x048 */ struct GphTex *src;
    /* 0x04C */ u8 pad4C[0x274];
} BtlTexTableRow; /* size 0x2C0 */

/* Texture slot remapping of model data (tbl: gphTexTableBuf; src: the
 * unit's model pack, BtlCharDef.modelPack). */
void BtlChrTexRemap(struct ChrModelData *d, struct GphTexTable *tbl, struct ChrModelData *src);        /* 0x006B38A8 */
struct GphTex *BtlChrTexFindById(struct GphTexTable *tbl, struct ChrModelData *src, s32 id);          /* 0x006B3A68 */
void BtlChrTexRemapToSlots(struct ChrModelData *d, struct GphTexTable *tbl, struct ChrModelData *src); /* 0x006B3AC8 */
u8 BtlChrTexFindSlotById(struct GphTexTable *tbl, struct ChrModelData *src, s32 id);                  /* 0x006B3B48 */

#endif /* BATTLE_N31_H */
