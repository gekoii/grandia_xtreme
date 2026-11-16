#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

typedef struct {
    u8 pad0[0x50];
    CamFU lookAt0;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    u8 pad70[0x20];
    f32 pitch;
    f32 yaw;
    f32 roll;
    f32 dist;
    s32 mode;
    s16 shakeFlags;
} Cam; /* Camera with a CamFU at +0x50: through camField[0].lookAt (even
        * cast to CamFU *) the stores address camField+0x50 directly and the
        * base register goes */

void MemSet(void *, s32, s32);
#define CAM ((Cam *)camField) /* camField[0] */

void CamFieldInit(void) {
    MemSet(&camCtl, 0, sizeof(camCtl));
    MemSet(&camBlend, 0, 0x320);
    CAM->lookAt0.i = 0;
    CAM->mode = 0x39;
    CAM->pitch = 0.13962634f;
    CAM->unk54 = 0.0f;
    CAM->unk58 = 0.0f;
    CAM->unk5C = 0.0f;
    CAM->unk60 = 0.0f;
    CAM->unk64 = 0.0f;
    CAM->unk68 = 0.0f;
    CAM->unk6C = 0.0f;
    CAM->roll = CAM->lookAt0.f;
    CAM->yaw = CAM->lookAt0.f;
    CAM->dist = 800.0f;
    CAM->shakeFlags = 0;
    camShakeInterval = 0;
    camShakeCountdown = 0;
    CamFollowInit();
    camCtl.playerShown = 1;
}
