#include "common.h"
#include "protos/main.h"

extern void MemSet(s32, s32, s32);

s32 MemCalloc16C(s32 arg0) {
    s32 p;

    p = MemAlloc(arg0, 0x10);
    MemSet(p, 0, arg0);
    return p;
}
