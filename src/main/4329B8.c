#include "common.h"
#include "protos/main.h"

/* asm; matrix pointers passed as s32, as VuMtxStackGetTop returns them */
void VuMtxRotate(s32, s32);

void VuMtxStackRotate(s32 arg0) {
    VuMtxRotate(VuMtxStackGetTop(), arg0);
}
