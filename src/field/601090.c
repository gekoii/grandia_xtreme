#include "common.h"
#include "vu.h"
#include "protos/field.h"

void VuMtxSetRotX(FMATRIX *, s32);
void VuMtxSetRotZ(FMATRIX *, s32);
void VuMtxMul(s32, s32, FMATRIX *);

void MapMiniArrowMatrix(s32 arg0, f32 x, f32 y, f32 z) {
    s32 r[3];
    FMATRIX m0;
    FMATRIX m1;

    r[0] = x * 10430.378f;
    r[1] = y * 10430.378f;
    r[2] = z * 10430.378f;
    VuMtxSetRotX(&m0, r[0]);
    VuMtxRotY(arg0, r[1], &m0);
    VuMtxSetRotZ(&m1, r[2] - r[1]);
    VuMtxMul(arg0, arg0, &m1);
}
