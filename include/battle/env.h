#ifndef BATTLE_ENV_H
#define BATTLE_ENV_H

#include "common.h"
#include "vu.h"
#include "sys.h" /* SprColorQuad */

struct GphLightList; /* gph.h */
struct GphModel;

/*
 * BATTLE environment (Env*, 0x006B3E20-0x006B8B88): lights, scene dimming,
 * planar shadows, lens flare, the stage model and its clip zones. Names follow
 * the ENV debug pages (DbgEnvLightPage 0x006B5E58 "AMB/CHAR/GROUND", the
 * CONTRAST, LENSFLARE and FIELD pages).
 */

/* ------------------------------------------------------------------ */
/* Per-map environment records (GRXSYS +0x134)                         */
/* ------------------------------------------------------------------ */

/* One directional light of an env record. */
typedef struct EnvParamLight {
    /* 0x0 */ f32 intensity;
    /* 0x4 */ s16 color[4];
    /* 0xC */ s16 angle[2];        /* x, y (16-bit) */
} EnvParamLight; /* size 0x10 */

/* Env record (EnvParamGet), 0x70 bytes. */
typedef struct EnvParam {
    /* 0x00 */ s16 amb[2][4];      /* [0] characters, [1] ground */
    /* 0x10 */ EnvParamLight lights[4]; /* [0-1] characters, [2-3] ground */
    /* 0x50 */ s16 shadowColor[4]; /* -> envLight.shadowColor (EnvLightInit; that view calls it fog) */
    /* 0x58 */ s16 fogColor[3];    /* -> btlCamera fog colour; 6ADD10 reads the low bytes as u8 */
    /* 0x5E */ u8 pad5E[2];
    /* 0x60 */ s16 fogDist;        /* -> Camera.fogDist */
    /* 0x62 */ s16 fogMax;         /* -> Camera.fogMax */
    /* 0x64 */ s16 fogMin;         /* -> Camera.fogMin */
    /* 0x66 */ s16 flareDist;      /* -> envFlareDistance */
    /* 0x68 */ s16 flareAngleX;    /* -> envFlareAngleX */
    /* 0x6A */ s16 flareAngleY;    /* -> envFlareAngleY; also the victory camera yaw */
    /* 0x6C */ f32 flareIntensity; /* -> envFlareIntensity */
} EnvParam; /* size 0x70 */

/* envParamTable: count, count map numbers, then count EnvParam records. */
typedef struct EnvParamTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 mapNos[1];       /* [count] */
} EnvParamTable;

/*
 * envParamTable is an EnvParamTable * (GRXSYS +0x134, not relocated), but
 * BtlDataLoad (0x006BC620) only matches with an s32 store (a pointer store
 * reorders the next GRXSYS load), so readers cast:
 * ((EnvParamTable *)envParamTable)->count (0x006B4930 matches that way).
 */
#define ENV_PARAM_TABLE ((EnvParamTable *)envParamTable)
extern s32 envParamTable;          /* 0x006E3ED8 */
extern s32 envParamIndex;          /* 0x006F3C08: current record (-1 until EnvParamGet) */

/* ------------------------------------------------------------------ */
/* Lights                                                              */
/* ------------------------------------------------------------------ */

/* envLight.lightFlags[] */
#define ENV_LIGHT_USED   0x800000
#define ENV_LIGHT_DIR    0x400000
#define ENV_LIGHT_GROUND 0x20000
#define ENV_LIGHT_CHAR   0x10000

typedef struct EnvLightAngles {
    /* 0x0 */ s32 x[2];
    /* 0x8 */ s32 y[2];
} EnvLightAngles; /* size 0x10 */

/*
 * Battle light setup (0x006F61B0). Index [0] is the character set, [1] the
 * ground set (the debug page's CHAR and GROUND columns); the second index
 * is the directional light. Two views (6B5788, 6B5E58) name the sets
 * col/col2, dir/dir2, its/its2, ax/ay/ax2/ay2.
 */
typedef struct EnvLight {
    /* 0x000 */ FVECTOR amb[2];       /* "AMB" */
    /* 0x020 */ FVECTOR color[2][2];
    /* 0x060 */ FVECTOR dir[2][2];
    /* 0x0A0 */ FVECTOR shadowColor;  /* envShadowColor; views: fog, a {s128; f32[4]} union in 684C00 */
    /* 0x0B0 */ f32 intensity[2][2];
    /* 0x0C0 */ EnvLightAngles angles[2]; /* 16-bit angles */
    /* 0x0E0 */ struct GphLightList *lists[3]; /* char, char dimmed, ground (envLightListChar...); views: void *, s32 */
    /* 0x0EC */ s32 lightFlags[16];   /* ENV_LIGHT_* per light slot (envLightFlags) */
    /* 0x12C */ u8 pad12C[4];
} EnvLight; /* size 0x130 (btlCamParam follows) */

extern EnvLight envLight;          /* 0x006F61B0 */
extern FVECTOR envShadowColor;     /* 0x006F6250 = envLight.shadowColor */
extern struct GphLightList *envLightListChar;    /* 0x006F6290 = envLight.lists[0] */
extern struct GphLightList *envLightListCharDim; /* 0x006F6294 = envLight.lists[1] */
extern struct GphLightList *envLightListGround;  /* 0x006F6298 = envLight.lists[2] */
extern s32 envLightFlags[16];      /* 0x006F629C = envLight.lightFlags */
extern s32 envLightListSel;        /* 0x006F3A90: 0 normal, 1 dimmed, -1 after EnvDimApply */
extern s32 dbgBtlEnvLightNo;       /* 0x006DF43C */

/* Scene dimming (EnvDim*): light level ramp. */
typedef struct EnvDim {
    /* 0x0 */ f32 cur;             /* 1 normal; 3 views declare only this (f32 envDim) */
    /* 0x4 */ f32 step;
    /* 0x8 */ f32 target;
    /* 0xC */ s32 mask;            /* envDimMask: units kept lit */
} EnvDim; /* size 0x10 */

extern EnvDim envDim;              /* 0x006F3A80 */
extern s32 envDimMask;             /* 0x006F3A8C = envDim.mask */
extern f32 envContrastDir;         /* 0x006F3C0C "DIR" */
extern f32 envContrastAmb;         /* 0x006F3C10 "AMB" */

/* ------------------------------------------------------------------ */
/* Shadows                                                             */
/* ------------------------------------------------------------------ */

typedef void (*EnvShadowCb)(s32);

extern FMATRIX envShadowMtx;       /* 0x006F3AC0 planar shadow projection */
extern f32 envShadowLightBlend;    /* 0x006F3B00 */
extern s32 envShadowLightIdx;      /* 0x006F3B04 */
extern s32 envShadowDrawLists[16]; /* 0x006F3B08 one per unit */
extern EnvShadowCb envShadowPreCb[16];  /* 0x006F3B48; s32[16] in 6B7368, 6B3E20 */
extern EnvShadowCb envShadowDrawCb[16]; /* 0x006F3B88; s32[16] in 6B3E20 */
extern s32 envShadowCbArg[16];     /* 0x006F3BC8 */

/* ------------------------------------------------------------------ */
/* Lens flare                                                          */
/* ------------------------------------------------------------------ */

extern f32 envFlareAlpha;          /* 0x006F3C20 "A" */
extern f32 envFlareAlphaStep;      /* 0x006F3C24; s32 in 6B77E0 */
extern f32 envFlareIntensity;      /* 0x006F3C28 */
extern f32 envFlareDistance;       /* 0x006F3C2C "DST" */
extern s32 envFlareAngleX;         /* 0x006F3C30 "RX" */
extern s32 envFlareAngleY;         /* 0x006F3C34 "Y" */
extern FVECTOR envFlarePos;        /* 0x006F3C40; views: Vec4 {x, y, z, w} */

/* ------------------------------------------------------------------ */
/* Stage model and clip zones                                          */
/* ------------------------------------------------------------------ */

/* Stage clip zone: parts hidden when the camera is inside (0x10). */
typedef struct EnvStageZone {
    /* 0x0 */ s32 angle;
    /* 0x4 */ s32 width;
    /* 0x8 */ s32 pitch;
    /* 0xC */ void *part;          /* views: Node *, void * */
} EnvStageZone; /* size 0x10 */

extern s32 envStageDrawList;       /* 0x006F3C50 */
extern EnvStageZone *envStageZones; /* 0x006F3C54 */
extern s32 envStageZoneCount;      /* 0x006F3C58 */
extern struct GphModel *envStageModel;    /* 0x006F3C5C; s32 in 6B8258 */
extern SprColorQuad envStageShadeQuad; /* 0x006F3C60 full-screen shade */
extern f32 envStageShadeAlpha;     /* 0x006F3C78 */
extern f32 envStageShadeStep;      /* 0x006F3C7C */
extern f32 envStageShadeTarget;    /* 0x006F3C80 */
extern s8 envStageClipOn;          /* 0x006DF440 "CLIP" */
extern s8 envStageZoneDisp;        /* 0x006DF441 "CHK_DISP" */

#endif /* BATTLE_ENV_H */
