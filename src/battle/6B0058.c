#include "common.h"
#include "vu.h"
#include "camera.h"
#include "battle/camera.h"

void BtlCamQuakeSetVec(VuVec3 *a, VuVec3 *b, s32 arg2) {
    Camera *p = btlCamera;

    if (!(p->shakeFlags & 1)) {
        p->shakeFlags |= 1;
        p->shakeMode = 4;
        p->shakeTick = 0;
        p->shakeCycle[0] = 4.0f;
        p->shakeCycle[1] = 4.0f;
        p->shakeCycle[2] = 4.0f;
        p->shakeCycle[3] = 4.0f;
        p->unk120[0] = 0;
        p->unk120[1] = 0;
        p->unk120[2] = 0;
        p->unk120[3] = 0;
        btlCamQuakeInterval = 0;
    }
    if (arg2 <= 0) {
        p->shakePeriod = 0;
        p->shakeAmp[0] = a->x;
        p->shakeAmp[1] = a->y;
        p->shakeAmp[2] = a->z;
        p->shakeAmp[3] = a->z;
        p->shakeRand[0] = b->x;
        p->shakeRand[1] = b->y;
        p->shakeRand[2] = b->z;
        p->shakeRand[3] = b->z;
    } else {
        p->shakeAmp2[0] = a->x;
        p->shakePeriod = arg2;
        p->shakeMode |= 8;
        p->shakeAmp2[1] = a->y;
        p->shakeAmp2[2] = a->z;
        p->shakeAmp2[3] = a->z;
        p->shakeRand2[0] = b->x;
        p->shakeRand2[1] = b->y;
        p->shakeRand2[2] = b->z;
        p->shakeRand2[3] = b->z;
    }
}
