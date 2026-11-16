#include "common.h"

void VuMtxSetRotZ(void *);
void VuMtxMul(s32, void *, s32);

void VuMtxRotZ(s32 arg0, s32 arg1, s32 arg2) {
    u8 buf[64];
    VuMtxSetRotZ(buf);
    VuMtxMul(arg0, buf, arg2);
}
