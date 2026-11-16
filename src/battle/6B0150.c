#include "common.h"
#include "battle/camera.h"

void BtlCamQuakeStart(f32 arg0) {
    Camera *p = btlCamera;
    s32 v;
    f32 t;

    p->shakeMode = 0x18;
    p->shakeCycle[1] = 2.0f;
    p->shakeFlags |= 1;
    p->shakeTick = 0;
    btlCamQuakeInterval = 1;
    v = (u32)(arg0 + 8.0f);
    p->shakeTime = v;
    p->shakePeriod = v;
    p->shakeAmp[0] = 0;
    t = arg0 * 5.0f;
    p->shakeAmp[1] = t;
    p->shakeRand[1] = t;
    p->shakeAmp[2] = 0;
    p->shakeAmp[3] = 0;
    p->shakeRand[0] = 0;
    p->shakeRand[2] = 0;
    p->shakeRand[3] = 0;
    p->shakeAmp2[0] = 0;
    p->shakeAmp2[1] = 0;
    p->shakeAmp2[2] = 0;
    p->shakeAmp2[3] = 0;
    p->shakeRand2[0] = 0;
    p->shakeRand2[1] = 0;
    p->shakeRand2[2] = 0;
    p->shakeRand2[3] = 0;
}
