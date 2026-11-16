#include "common.h"

void VuMtxSetRotX(s32 *);
void VuMtxMul(s32, s32 *, s32);

void VuMtxRotX(s32 arg0, s32 arg1, s32 arg2) {
    s32 buf[16];
    VuMtxSetRotX(buf);
    VuMtxMul(arg0, buf, arg2);
}
