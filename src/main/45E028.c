#include "common.h"
#include "protos/main.h"

void VuMtxMul(s32 a, s32 b, s32 c);

void VuMtxRotZYX(s32 arg0, s32 arg1, s32 arg2) {
    VuMtxSetRotZYX(arg0, arg1);
    VuMtxMul(arg0, arg0, arg2);
}
