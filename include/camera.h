#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

/*
 * The engine camera (main 0x00431110-0x00431B10: CamInit, CamApply,
 * CamCalcMatrix, CamDirToAngles/CamAnglesToDir, CamCalcDir/Up/Roll,
 * CamUpdateShake/CamCalcShake). FIELD (camField[2], camCtl.saved), BATTLE
 * (btlCameraWork via btlCamera) and VIEWER (vwrCamera) all use it.
 *
 * Size: 0x180. CamInit writes up to +0x160 (zoom); FIELD's camField array has a
 * 0x180 stride (camField[1].pitch is +0x210 in 0x00601138) and camCtl saves a
 * whole camera at +0x50 of its 0x1D0 bytes, copied as 0x30 doublewords
 * (0x005C9F28, 0x005C9FD0).
 *
 * Angles are radians (VuSinCosAngle gets angle * 10430.378 = 16-bit units).
 * FIELD calls lookAt "MOVE" and eye "POS" on its camera debug pages.
 */

/* Sony's sceVu0FVECTOR. */
#include "vu.h"

/* Camera.mode: which vectors CamCalcMatrix derives from which. */
#define CAM_MODE_FOLLOW 0x23 /* CamInit default */
#define CAM_MODE_FIXED  0x33
#define CAM_MODE_NORMAL 0x39 /* CamDebugReset, FIELD default */

/* Camera.shakeFlags */
#define CAM_SHAKE_ON 0x1

typedef struct Camera {
    /* 0x000 */ FVECTOR viewMtx[4];    /* built by CamCalcMatrix */
    /* 0x040 */ FVECTOR shakeOfs;      /* CamCalcShake's offset, cleared by CamUpdateShake */
    /* 0x050 */ FVECTOR lookAt;        /* point looked at ("MOVE"; fldCamMovePos) */
    /* 0x060 */ FVECTOR eye;           /* camera position ("POS"; fldCameraPos) */
    /* 0x070 */ FVECTOR dir;           /* lookAt - eye, normalised (CamCalcDir) */
    /* 0x080 */ FVECTOR up;            /* CamCalcUp / CamCalcRoll */
    /* 0x090 */ f32 pitch;
    /* 0x094 */ f32 yaw;               /* fldCameraYaw */
    /* 0x098 */ f32 roll;
    /* 0x09C */ f32 dist;              /* |lookAt - eye| ("LENGTH") */
    /* 0x0A0 */ s32 mode;              /* CAM_MODE_*; camFieldMode */
    /* 0x0A4 */ u16 shakeFlags;        /* CAM_SHAKE_ON */
    /* 0x0A6 */ u16 shakeMode;         /* 4, 8 periodic part on, 0x10 timed */
    /* 0x0A8 */ s32 shakeTime;         /* frames left when shakeMode & 0x10 */
    /* 0x0AC */ s32 shakeTick;
    /* 0x0B0 */ s32 shakePeriod;       /* CamSetShakePeriod */
    /* 0x0B4 */ u8 padB4[0xC];
    /* 0x0C0 */ FVECTOR shakeAmp;      /* random part: base amplitude (CamSetShakeAmp) */
    /* 0x0D0 */ FVECTOR unkD0;         /* cleared by CamInit */
    /* 0x0E0 */ FVECTOR shakeRand;     /* random part: random amplitude */
    /* 0x0F0 */ FVECTOR shakeCycle;
    /* 0x100 */ FVECTOR shakeAmp2;     /* periodic part (CamSetShakePeriod) */
    /* 0x110 */ FVECTOR shakeRand2;
    /* 0x120 */ FVECTOR unk120;        /* cleared by CamInit and BtlCamQuake* */
    /* 0x130 */ s32 viewNo;            /* view (GphView*) the camera drives */
    /* 0x134 */ s32 unk134;
    /* 0x138 */ s32 unk138;
    /* 0x13C */ s32 screenW;           /* 640 */
    /* 0x140 */ s32 screenH;           /* 448 */
    /* 0x144 */ u8 fogColor[3];
    /* 0x147 */ u8 pad147;
    /* 0x148 */ f32 clipFar;           /* 8000 */
    /* 0x14C */ f32 clipNear;          /* 1 */
    /* 0x150 */ f32 fov;               /* degrees, 55 ("PERS") */
    /* 0x154 */ f32 fogMin;            /* GphViewSetFog's third value, 0 */
    /* 0x158 */ f32 fogMax;            /* 255 */
    /* 0x15C */ f32 fogDist;           /* 10000, GRX.INI fog dist */
    /* 0x160 */ f32 zoom;              /* scales fov (GphViewSetFovScaled), 1 */
    /* 0x164 */ u8 pad164[0x1C];
} Camera; /* size 0x180 */

/*
 * A float slot stored as an int and read back as a float: CamDebugReset
 * (0x0040B958) and CamFieldInit (0x005C98B0) store an int 0 to lookAt[0] and
 * copy it to the angles as a float; the access has to go through a union for
 * the load to stay after the store. Through a camera pointer
 * `((CamFU *)cam->lookAt)->i = 0; ... = ((CamFU *)cam->lookAt)->f;` matches
 * (CamDebugReset); CamFieldInit works on the global camField[0] and keeps a
 * local view with a CamFU member at +0x50 (through camField[0].lookAt the
 * stores address camField+0x50 directly instead of base + 0x50).
 */
typedef union CamFU {
    f32 f;
    s32 i;
} CamFU;

/*
 * Codegen constraints found when switching files to this header:
 * - shakeTime/shakePeriod are read as u32 in 0x006B02A8 (unsigned-to-float
 *   conversion); s32 matches there with a (u32) cast at the use.
 * - 0x00654058/0x00655278 store an int 0 to roll: same `sw $0` as 0.0f, no
 *   union needed.
 *
 * Shake vectors: several BATTLE files (BtlCamQuake* 0x006AFFA0, 0x006B0150,
 * 0x006B0218) store the components of shakeAmp..unk120 as s32 (zeroes) next
 * to f32 values; CamCalcShake reads them as floats.
 */

extern Camera *btlCamera;     /* main; BATTLE points it at btlCameraWork */
extern Camera btlCameraWork;  /* BATTLE */
extern Camera vwrCamera;      /* VIEWER; VwrInit (0x00700058) clears 0x600 bytes
                               * here (4 x 0x180): the 0x480 after it is other
                               * viewer state */

#endif /* CAMERA_H */
