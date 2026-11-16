#include "common.h"
#include "main/n06.h"
#include "protos/main.h"

typedef struct {
    f32 x, y, z, w;
    f32 s, t;
    u32 color;
    u32 pad1C;
} Vert;

s64 GsPackST(f32, f32);
s64 __fixunssfdi(f32);

void GphTexPrim2DBuild(GphPrimPkt *p, u64 arg1, s32 n, Vert *q0) {
    s32 k;
    s32 i;
    u64 *d;
    u64 x;
    Vert *v;

    k = ((n * 3 + 7) & 0xFFFE) >> 1;
    p->dmaTag = 0x10000000 | (k + 1);
    p->vifDirect = k | 0x50000000;
    p->gifTag = (arg1 << 47) | 0x1000400000008001;
    p->gifRegs = 0xE;
    p->reg0 = 0x58;
    p->reg1 = 0x1B;
    p->gifTag2 = (n | 0x8000) | 0x3400000000000000;
    p->gifRegs2 = 0x512;
    p->unk8 = 0;
    p->vif0 = 0;
    p->vif1 = 0;
    p->vif2 = 0;
    d = p->data;
    v = q0;
    for (i = 0; i < n; i++) {
        *d++ = GsPackST(v->s, v->t);
        *d++ = (u64)v->color | 0x3F80000000000000;
        x = (__fixsfdi(v->x * 16.0f) + 0x8000) & 0xFFFF;
        x |= ((__fixsfdi(v->y * 16.0f) + 0x8000) & 0xFFFF) << 16;
        x |= __fixunssfdi(v->z) << 32;
        v++;
        *d++ = x;
    }
    if (n & 1) {
        *d = 0;
    }
}
