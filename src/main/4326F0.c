#include "common.h"
#include "vu.h"

extern void VuMtxMul(FMATRIX *, s32, FMATRIX *);

void VuMtxArrayMulLeft(s32 arg0, FMATRIX *arg1, u32 arg2) {
    u32 i;

    for (i = 0; i < arg2; i++) {
        VuMtxMul(&arg1[i], arg0, &arg1[i]);
    }
}
