#include "common.h"
#include "field/state.h"

void CamShakeStop(s32 n) {
    Camera *c = &camField[0];

    if (n <= 0) {
        c->shakeAmp[0] = 0;
        c->shakeFlags &= ~1;
        c->shakeAmp[1] = 0;
        c->shakeAmp[2] = 0;
        c->shakeAmp[3] = 0;
        c->shakeRand[0] = 0;
        c->shakeRand[1] = 0;
        c->shakeRand[2] = 0;
        c->shakeRand[3] = 0;
        c->shakeTick = 0;
        return;
    }
    c->shakeAmp2[0] = 0;
    c->shakeMode |= 0x18;
    c->shakeAmp2[1] = 0;
    c->shakeAmp2[2] = 0;
    c->shakeAmp2[3] = 0;
    c->shakeRand2[0] = 0;
    c->shakeRand2[1] = 0;
    c->shakeRand2[2] = 0;
    c->shakeRand2[3] = 0;
    c->shakeTime = n;
    c->shakePeriod = n;
}
