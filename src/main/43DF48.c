#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"


void GphTexPrim3DDraw(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;

    temp_v0 = GsPacketAlloc64((arg1 * 0x18) + 0x60);
    GphTexPrim3DBuild(temp_v0, arg0, arg1, arg2);
    GphPrimListAdd(temp_v0);
}
