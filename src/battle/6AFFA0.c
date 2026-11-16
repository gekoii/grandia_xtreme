#include "common.h"
#include "camera.h"
#include "battle/camera.h"

void BtlCamQuakeSetY(s32 arg0, f32 a, f32 b) {
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
    if (arg0 <= 0) {
        p->shakeAmp[1] = a;
        p->shakeRand[1] = b;
        p->shakeAmp[0] = 0;
        p->shakeAmp[2] = 0;
        p->shakeAmp[3] = 0;
        p->shakeRand[0] = 0;
        p->shakeRand[2] = 0;
        p->shakeRand[3] = 0;
        p->shakePeriod = 0;
    } else {
        p->shakePeriod = arg0;
        p->shakeAmp2[1] = a;
        p->shakeMode |= 8;
        p->shakeRand2[1] = b;
        p->shakeAmp2[2] = 0;
        p->shakeAmp2[3] = 0;
        p->shakeRand2[0] = 0;
        p->shakeRand2[2] = 0;
        p->shakeRand2[3] = 0;
        p->shakeAmp2[0] = 0;
    }
}
