#include "common.h"
#include "protos/main.h"

extern void MemSet(void *, s32, s32);

void *MemCalloc32(s32 arg0) {
    void *p;

    p = MemAlloc(arg0, 0x20);
    MemSet(p, 0, arg0);
    return p;
}
