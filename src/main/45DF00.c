#include "common.h"
#include "protos/main.h"

void VuMtxSetRotX(s32 a, s32 b);

void VuMtxSetRotXYZ(s32 arg0, s32 *arg1) {
    VuMtxSetRotX(arg0, arg1[2]);
    VuMtxRotY(arg0, arg1[1], arg0);
    VuMtxRotZ(arg0, arg1[0], arg0);
}
