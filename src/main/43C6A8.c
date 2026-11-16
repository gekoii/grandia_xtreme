#include "common.h"
#include "main/n06.h"
#include "protos/main.h"

extern s64 __fixunssfdi(f32);

void GphLine2DDraw(u32 arg0, VuVec4 *arg1, u32 arg2, s32 arg3) {
    GphPrimPkt *p;
    u32 k;
    u32 n;
    u64 *d;
    VuVec4 *v;
    u32 i;
    u64 x;

    n = (arg0 + 1) >> 1;
    k = n + 4;
    p = GsPacketAlloc64((k + 1) * 16);
    if (p == NULL) {
        return;
    }
    p->dmaTag = (u32)(k | 0x10000000);
    p->unk8 = 0;
    p->vif0 = 0;
    p->vif1 = 0;
    p->vif2 = 0;
    k = n + 3;
    p->vifDirect = (k & 0xFFFF) | 0x50000000;
    p->gifRegs = 0x10;
    p->gifTag = 0x2400000000000001;
    p->reg0 = (arg3 & 1) ? 2 : 1;
    p->reg1 = (u64)arg2 | 0x3F80000000000000;
    p->gifRegs2 = 4;
    p->gifTag2 = (u64)(arg0 | 0x8000) | 0x1400000000000000;
    v = arg1;
    d = p->data;
    for (i = 0; i < arg0; i++) {
        x = (__fixsfdi(v->x * 16.0f) + 0x8000) & 0xFFFF;
        x |= ((__fixsfdi(v->y * 16.0f) + 0x8000) & 0xFFFF) << 16;
        x |= __fixunssfdi(v->z) << 32;
        v++;
        *d++ = x;
    }
    if (arg0 & 1) {
        *d = 0;
    }
    GphPrimListAdd(p);
}
