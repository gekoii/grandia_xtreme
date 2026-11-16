#ifndef BATTLE_CAMERA_H
#define BATTLE_CAMERA_H

#include "common.h"
#include "vu.h"
#include "../camera.h" /* the engine Camera (not this file) */
#include "battle/unit.h"

/*
 * BATTLE camera control (BtlCam*, 0x006ADD10-0x006B0DA8). The camera itself
 * is the engine Camera (camera.h: btlCamera -> btlCameraWork); each frame
 * BtlCamApply (0x006ADF98) copies btlCamParam into it. Field names follow the
 * CAMERA debug page (0x006B02A8): "DST", "TGT", "CMR", "ROT", "ANG", "MODE",
 * "FLAG", "PHASE", "COUNT".
 */

/* btlCamParam.mode (BtlCamSetMode 0x006AE2F8) */
#define BTL_CAM_CUSTOM   0
#define BTL_CAM_DEFAULT  1
#define BTL_CAM_UNIT     2
#define BTL_CAM_UNIT_TGT 3
#define BTL_CAM_CLOSEUP  4
#define BTL_CAM_PAIR     5
#define BTL_CAM_FAR      6
#define BTL_CAM_INTRO    7
#define BTL_CAM_TARGETS  8
#define BTL_CAM_UNIT2    9

/* Camera parameters (0x006F62E0). */
typedef struct BtlCamParam {
    /* 0x00 */ s8 mode;            /* "MODE": BTL_CAM_*; views: u8 x5 */
    /* 0x01 */ u8 flags;           /* "FLAG"; views: s8 x4 */
    /* 0x02 */ s8 subMode;
    /* 0x03 */ s8 unit;            /* btlCamParamUnit: member followed in modes 2/3 */
    /* 0x04 */ s32 unk4;           /* one view calls it ang */
    /* 0x08 */ s32 count;          /* "COUNT" */
    /* 0x0C */ s32 phase;          /* "PHASE"; one view calls it count */
    /* 0x10 */ BtlUnit *unitPtr;   /* BtlCamSetMode's unit / proc argument; one view void * */
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ f32 dist;           /* "DST" (btlCamDistance) */
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ FVECTOR target;     /* "TGT": looked-at point; views: Vec4 / f32 x, y, z */
    /* 0x30 */ FVECTOR eye;        /* "CMR": camera position (btlCamParamCmrPos) */
    /* 0x40 */ s32 rot[3];         /* "ROT": 16-bit pitch, yaw (btlCamYaw), roll */
    /* 0x4C */ f32 fov;            /* "ANG" */
} BtlCamParam; /* size 0x50 (btlDebugCfg follows) */

extern BtlCamParam btlCamParam;    /* 0x006F62E0 */
extern s8 btlCamParamUnit;         /* 0x006F62E3 = btlCamParam.unit */
extern f32 btlCamDistance;         /* 0x006F62F8 = btlCamParam.dist */
extern FVECTOR btlCamParamCmrPos;  /* 0x006F6310 = btlCamParam.eye */
extern s32 btlCamYaw;              /* 0x006F6324 = btlCamParam.rot[1] */

/* A camera pose (BtlCamCtrl.from / .to / .ofs); same order as BtlCamParam +0x20. */
typedef struct BtlCamPose {
    /* 0x00 */ FVECTOR target;
    /* 0x10 */ FVECTOR eye;
    /* 0x20 */ s32 rot[3];
    /* 0x2C */ f32 dist;
    /* 0x30 */ f32 fov;
    /* 0x34 */ u8 pad34[0xC];
} BtlCamPose; /* size 0x40 */

struct BtlCamCtrl;
typedef void (*BtlCamProc)(struct BtlCamCtrl *, BtlCamParam *);

/* Camera controller (0x006F2CE0): blends btlCamParam from one pose to another. */
typedef struct BtlCamCtrl {
    /* 0x00 */ s32 frames;         /* frames left */
    /* 0x04 */ s32 duration;       /* blend duration */
    /* 0x08 */ BtlCamProc proc;    /* mode proc; views: void * */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ BtlCamPose from;
    /* 0x50 */ BtlCamPose to;
    /* 0x90 */ BtlCamPose ofs;     /* smoothed offsets that the overview / behind-unit procs (0x006AEB50,
                                    * 0x006AEF98, 0x006AF9A8) add to the pose they write to .to:
                                    * target x/z, rot, dist */
} BtlCamCtrl; /* size 0xD0 (btlCamDefaultProc follows) */

extern BtlCamCtrl btlCamCtrl;      /* 0x006F2CE0 */
extern BtlCamProc btlCamDefaultProc; /* 0x006F2DB0: mode-1 proc */
extern BtlCamProc btlCamIntroProc; /* 0x006F2DB4: mode-7 proc */
extern f32 btlCamChaseVelX;        /* 0x006F2DB8 */
extern f32 btlCamChaseVelZ;        /* 0x006F2DBC */
extern f32 btlCamChaseOfsX;        /* 0x006F2DC0 */
extern BtlUnit *btlCamBossUnit;    /* 0x006F2DC4: first enemy in boss encounters; views: void * */

extern s8 btlCamQuakeInterval;     /* 0x006DF438 "INT"; u8 in 6ADF98, 6B02A8 (which match with (u8) casts) */
extern s8 btlCamQuakeWait;         /* 0x006DF439; u8 in 6ADF98 */
extern f32 btlStartCamPanDist;     /* 0x006F2228 */
extern s32 btlVictoryCamYaw;       /* 0x006E3EF4 */
extern f32 btlVictoryCamDist;      /* 0x006F3E7C */

#endif /* BATTLE_CAMERA_H */
