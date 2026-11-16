#include "common.h"
#include "protos/main.h"

void MemSet(s32 a, s32 b, s32 c);

s32 MemCalloc16B(s32 arg0) {
    s32 v;

    v = MemAlloc(arg0, 0x10);
    MemSet(v, 0, arg0);
    return v;
}
