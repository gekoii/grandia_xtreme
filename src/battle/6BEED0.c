#include "common.h"
#include "battle/unit.h"

void VuSinCosAngle(f32 *, f32 *, s32);

void BtlUnitShakeOffset(BtlUnit *o, f32 r) {
    f32 v[2];
    s32 k;
    s32 m;
    f32 nr;

    k = o->stateCount / 2;
    m = k & 3;
    if (k & 1) {
        o->unkD0[0] = o->unkD0[2] = 0.0f;
        return;
    }
    VuSinCosAngle(&v[0], &v[1], o->rot[1]);
    if (m < 2) {
        o->unkD0[0] = r * v[0];
        o->unkD0[2] = r * v[1];
        return;
    }
    nr = -r;
    o->unkD0[0] = nr * v[0];
    o->unkD0[2] = nr * v[1];
}
