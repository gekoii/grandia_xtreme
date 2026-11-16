#include "common.h"
#include "protos/main.h"

void VuMtxSetRotZ(s32, s32);

void VuMtxSetRotZYX(s32 arg0, s32 *arg1) {
    VuMtxSetRotZ(arg0, arg1[2]);
    VuMtxRotY(arg0, arg1[1], arg0);
    VuMtxRotX(arg0, arg1[0], arg0);
}
