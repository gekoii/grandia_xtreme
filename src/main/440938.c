#include "common.h"
#include "protos/main.h"

extern void MemSet(void *, s32, s32);

void *MemCalloc64(s32 arg0) {
    void *p = MemAlloc(arg0, 0x40);
    MemSet(p, 0, arg0);
    return p;
}
