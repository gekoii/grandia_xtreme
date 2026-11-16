#include "common.h"
#include "protos/main.h"

/* asm; matrix pointers passed as s32, as VuMtxStackGetTop returns them */
void VuMtxRotateQuat(s32, s32);

void VuMtxStackRotateQuat(s32 arg0) {
    VuMtxRotateQuat(VuMtxStackGetTop(), arg0);
}
