#include "common.h"
#include "field/state.h"
#include "field/n15.h"

s32 fptoui(f32);

void CamShakeStartQuake(f32 f) {
    s32 t;

    camField[0].shakeMode = 0x18;
    camField[0].shakeFlags |= 1;
    camField[0].shakeCycle[1] = 4.0f;
    camField[0].shakeTick = 0;
    camShakeInterval = 0;
    t = fptoui((f + 8.0f) * 2.0f);
    camField[0].shakeRand2[3] = 0.0f;
    camField[0].shakeTime = t;
    camField[0].shakeAmp[0] = 0.0f;
    camField[0].shakeAmp[1] = f * 5.0f;
    camField[0].shakeRand[1] = f * 5.0f;
    camField[0].shakeAmp[2] = 0.0f;
    camField[0].shakeAmp[3] = 0.0f;
    camField[0].shakeRand[0] = 0.0f;
    camField[0].shakeRand[2] = 0.0f;
    camField[0].shakeRand[3] = 0.0f;
    camField[0].shakeAmp2[0] = 0.0f;
    camField[0].shakeAmp2[1] = 0.0f;
    camField[0].shakeAmp2[2] = 0.0f;
    camField[0].shakeAmp2[3] = 0.0f;
    camField[0].shakeRand2[0] = 0.0f;
    camField[0].shakeRand2[1] = 0.0f;
    camField[0].shakeRand2[2] = 0.0f;
    camField[0].shakePeriod = t;
}
