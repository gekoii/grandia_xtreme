#include "common.h"

extern void VuMtxMul(s32, s32, s32);

void VuMtxArrayMulRight(s32 arg0, s32 arg1, u32 arg2) {
    u32 i;

    for (i = 0; i < arg2; i++) {
        VuMtxMul(arg1 + i * 0x40, arg1 + i * 0x40, arg0);
    }
}
