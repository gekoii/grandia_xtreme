#include "common.h"
#include "protos/main.h"

extern void MemSet(s32, s32, s32);

s32 MemCalloc128(s32 arg0) {
    s32 r;

    r = MemAlloc(arg0, 0x80);
    MemSet(r, 0, arg0);
    return r;
}
