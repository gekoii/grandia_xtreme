#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"

void CamShakeSetVec(VuVec4 *, VuVec4 *, s32);

void CamShakeStartPreset(f32 t) {
    VuVec4 a;
    VuVec4 b;
    s32 n;

    a.x = -40.0f;
    a.y = 20.0f;
    b.x = 80.0f;
    b.y = 40.0f;
    a.z = 0.0f;
    a.w = 0.0f;
    b.z = 0.0f;
    b.w = 0.0f;
    n = t;
    CamShakeSetVec(&a, &b, 0);
    camField[0].shakeCycle[0] = 4.0f;
    camField[0].shakeCycle[1] = 4.0f;
    camField[0].shakeCycle[2] = 0.0f;
    camField[0].shakeCycle[3] = 0.0f;
    CamShakeStop(n);
}
