#include "common.h"
#include "field/state.h"
#include "field/n15.h"

typedef struct {
    f32 x, y, z;
} Vec;

void CamShakeSetVec(Vec *a, Vec *b, s32 n) {
    if (!(camField[0].shakeFlags & 1)) {
        camField[0].shakeFlags |= 1;
        camField[0].shakeMode = 4;
        camField[0].shakeTick = 0;
        camField[0].shakeCycle[0] = 1.0f;
        camField[0].shakeCycle[2] = 1.0f;
        camField[0].shakeCycle[1] = 4.0f;
        camShakeInterval = 0;
        camField[0].shakeCycle[3] = 1.0f;
    }
    if (n <= 0) {
        camField[0].shakePeriod = 0;
        camField[0].shakeAmp[0] = a->x;
        camField[0].shakeAmp[1] = a->y;
        camField[0].shakeAmp[2] = a->z;
        camField[0].shakeAmp[3] = a->z;
        camField[0].shakeRand[0] = b->x;
        camField[0].shakeRand[1] = b->y;
        camField[0].shakeRand[2] = b->z;
        camField[0].shakeRand[3] = b->z;
    } else {
        camField[0].shakeAmp2[0] = a->x;
        camField[0].shakePeriod = n;
        camField[0].shakeMode |= 8;
        camField[0].shakeAmp2[1] = a->y;
        camField[0].shakeAmp2[2] = a->z;
        camField[0].shakeAmp2[3] = a->z;
        camField[0].shakeRand2[0] = b->x;
        camField[0].shakeRand2[1] = b->y;
        camField[0].shakeRand2[2] = b->z;
        camField[0].shakeRand2[3] = b->z;
    }
}
