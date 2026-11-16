#include "common.h"

void VuMtxSetRotY(void *);
void VuMtxMul(s32, void *, s32);

void VuMtxRotY(s32 arg0, s32 arg1, s32 arg2) {
    s32 buf[16];
    VuMtxSetRotY(buf);
    VuMtxMul(arg0, buf, arg2);
}
