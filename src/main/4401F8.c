#include "common.h"
#include "main/n06.h"

s32 MemInit(s32 arg0, s32 *arg1, s32 arg2) {
    u32 size;
    u32 n;
    u32 i;
    s32 *p;

    size = (arg0 + 1) << 5;
    memArenaEnd = arg2;
    n = size >> 2;
    memArenaCur = (u32)arg1;
    memHeapTable = (MemHeapTable *)arg1;
    p = arg1;
    for (i = 0; i < n; i++) {
        *p++ = 0;
    }
    {
        u32 q = memArenaCur + size;
        memHeapTable->count = arg0;
        memArenaCur = q;
    }
    return 0;
}
