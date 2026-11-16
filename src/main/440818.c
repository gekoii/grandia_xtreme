#include "common.h"
#include "protos/main.h"

void MemSet(s32, s32, s32);

s32 MemCalloc16A(s32 arg0) {
    s32 ret = MemAlloc(arg0, 0x10);
    MemSet(ret, 0, arg0);
    return ret;
}
