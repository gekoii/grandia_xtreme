#include "common.h"
#include "protos/main.h"

extern void VuMtxMul(s32, s32, s32);

void VuMtxRotXYZ(s32 arg0, s32 *arg1, s32 arg2) {
    VuMtxSetRotXYZ(arg0, arg1);
    VuMtxMul(arg0, arg0, arg2);
}
