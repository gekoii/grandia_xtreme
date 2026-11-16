#ifndef CHR_H
#define CHR_H

/* Character types of the main executable: model data (a model pack entry),
 * character packs (SYS/PARTY.PAC and the map packs) and their bindings, the
 * face animation, the loaded motion list and the debug MOTION page's
 * per-object settings. Shared by FIELD, BATTLE and the viewer. */

#include "common.h"
#include "vu.h"
#include "gph.h"

/* ------------------------------------------------------------ model data */

/* Model data (a model pack entry): ChrModelCreate 4381D8 builds a GphModel
 * instance from it, ChrModelCalcInstanceSize 438188 sizes one, and
 * ChrModelCopyTransform 437148 / ChrModelCopyPartAttr 4371F8 restore an
 * instance from it. */
typedef struct ChrModelData {
    /* 0x00 */ s32 partCount;
    /* 0x04 */ s32 gaObjCount;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ u16 flags;       /* -> GphModel.flags */
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ FVECTOR pos;     /* -> GphModel.pos/rot/scale */
    /* 0x20 */ IVECTOR rot;
    /* 0x30 */ FVECTOR scale;
    /* 0x40 */ FVECTOR cullSphere; /* -> GphModel.cullSphere */
    /* 0x50 */ GphPart *parts;  /* partCount parts, copied by ChrModelCreateParts 438378 */
    /* 0x54 */ GphGaObject *gaObjs; /* gaObjCount objects (BATTLE BtlChrTex* remap their texture slots) */
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ f32 unk5C;       /* a scale factor (results n05/n31); "scale" is taken by +0x30 */
    /* 0x60 */ s32 unk60;       /* -> GphModel.unk4 (after unk64) */
    /* 0x64 */ s32 unk64;
} ChrModelData; /* size >= 0x68 */

/* -------------------------------------------------------- character packs */

/* Character pack (SYS/PARTY.PAC or a map MDT pack; FilePacCharRelocate
 * 4538F0): non-zero offsets become addresses. s32 throughout, as the
 * relocation adds them (h->items[i] += (s32)h); readers that use them as
 * pointers cast. */
#ifndef CHR_PACK_DEFINED
#define CHR_PACK_DEFINED
typedef struct ChrPack {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ s32 modelPac;       /* model pack (FilePacModelRelocate, FilePacModelBindTextures) */
    /* 0x010 */ s32 enemyData;      /* PtyEnemyDataRelocate; byte 2 = map icon (FIELD 5F2548) */
    /* 0x014 */ s32 items[8];       /* face texture items; ChrPackBind 453B88 reads each one's id at +0x60 */
    /* 0x034 */ s32 motions[32];    /* chunks (FileChunkRelocate once each), +0x10; 0 = none */
    /* 0x0B4 */ s32 motionEvents[32]; /* per motion (BATTLE 6BCA58 reads motions[i] and motionEvents[i]) */
    /* 0x134 */ s32 unk134[4];
    /* 0x144 */ s32 modelPacks[8];  /* FilePacModelRelocate */
    /* 0x164 */ s32 animSets[8];    /* GphAnimSetRelocate */
    /* 0x184 */ s32 chunks[8];      /* FileChunkRelocate, +0x10 */
    /* 0x1A4 */ s32 unk1A4[8];
} ChrPack; /* size >= 0x1C4 */
#endif

/* Face texture ids of a bound pack (ChrPackBinding +8). ChrFaceAnim.faces
 * points here (ChrFaceAnimInit 453C10: binding + 8); the reads in
 * 453C40/453CB0 only match through this struct, so they cast faces to it. */
typedef struct ChrFaceIds {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 ids[8];         /* face texture ids by face, -1 = none */
} ChrFaceIds; /* size = 0x24 */

/* A bound character pack (ChrPackBind 453B88): FIELD fldPartyPackTable /
 * fldChrPackTable, the debug battle's dbgPartyModels/dbgEnemyModels, the
 * btlHandoff models (sys.h forward-declares the tag). */
#ifndef CHR_PACK_BINDING_DEFINED
#define CHR_PACK_BINDING_DEFINED
typedef struct ChrPackBinding {
    /* 0x00 */ ChrPack *pack;
    /* 0x04 */ s32 modelPac;       /* = pack->modelPac */
    /* 0x08 */ ChrFaceIds faces;   /* ids[i] = pack->items[i]'s id, -1 = none */
} ChrPackBinding; /* size 0x2C */
#endif

/* ------------------------------------------------------- face animation */

/* Face animation (ChrFaceAnim* 0x00453C10-0x00453CB0; BATTLE debug page
 * names). */
#ifndef CHR_FACE_ANIM_DEFINED
#define CHR_FACE_ANIM_DEFINED
typedef struct ChrFaceAnim {
    /* 0x0 */ u8 face;             /* "FACE ID": face (expression) 0-7 */
    /* 0x1 */ u8 mode;             /* "FACE MD": blink on */
    /* 0x2 */ u16 blinkStep;       /* "C1" */
    /* 0x4 */ u16 blinkTimer;      /* "C2": frames to the next blink (120-540), bit 3 kept */
    /* 0x6 */ u16 stepTimer;       /* "C3" */
    /* 0x8 */ s32 *faces;          /* &binding->faces (ChrFaceAnimInit); read as ChrFaceIds * */
} ChrFaceAnim; /* size 0xC */
#endif

/* ----------------------------------------------------------- motion list */

/* Loaded motions (VwrLoadMotionPack, FIELD's FldLoadMotionPack 0x005B1490).
 * The items are GphMotion pointers but s32 in every view: VwrLoadMotionPack
 * needs an int store (count is reloaded after it) and VwrUpdate an int load
 * (scheduled after a s16 store), so readers cast. */
#ifndef CHR_MOTION_LIST_DEFINED
#define CHR_MOTION_LIST_DEFINED
typedef struct ChrMotionList {
    /* 0x000 */ u32 count;
    /* 0x004 */ s32 items[256];    /* GphMotion * */
} ChrMotionList; /* size 0x404 */
#endif

extern ChrMotionList chrMotionList;     /* 0x004F25C8 */

/* Per-object motion settings of the debug MOTION page (dbgMotObjParams
 * 0x004F0AD8, 32 entries; reset by DbgViewInit 0x00420808, edited by
 * DbgMotionObjPage 0x00426940, used by the viewer). */
#ifndef DBG_MOT_OBJ_PARAM_DEFINED
#define DBG_MOT_OBJ_PARAM_DEFINED
typedef struct DbgMotObjParam {
    /* 0x0 */ s16 motionNo;     /* index into chrMotionList, -1 none (426940 reads it as u16) */
    /* 0x2 */ u8 mode;          /* 0 stop, 1 run, 2 sequence (next motion at the end) */
    /* 0x3 */ u8 flag;          /* entries 0-2: CRS/SDW/STG display toggles (GRX.INI "viewer
                                 * crs/sdw/stg"); entry 3: motion blend frames */
    /* 0x4 */ u8 unk4;          /* set to 1 by DbgViewInit */
    /* 0x5 */ u8 pad5[3];
    /* 0x8 */ f32 speed;        /* frame step */
} DbgMotObjParam; /* size 0xC */
#endif

extern DbgMotObjParam dbgMotObjParams[32]; /* 0x004F0AD8 */

#endif
