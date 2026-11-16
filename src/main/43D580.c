#include "common.h"
#include "protos/main.h"

void GsRenderStatePacketSet(s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s64);

void GphPrimAddGsState(void) {
    s32 obj;

    obj = GsPacketAlloc64(0x90);
    GsRenderStatePacketSet(obj, 1, 0, 0, 1, 0, 1, 0,
                  0, 0, 5, 0, 1, 0, 0, 1,
                  3, 0, 1, 0, 1, 0x80, 0, 0,
                  0, 0, 0, 0, 0, 0);
    GphPrimListAdd(obj);
}
