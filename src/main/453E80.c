#include "common.h"
#include "main/n08.h"

void GphScreenFxSetHold(GphScreenFx *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    GphScreenFxSet(arg0, arg1, arg2, arg3, arg4);
    arg0->flags |= 0x8000;
}
