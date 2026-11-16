#include "common.h"
#include "gph.h"
#include "protos/main.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void MemMove128(u8 *, u8 *, u32);

void GphMorphNormalsCopy(GphPart *g, u8 *arg1) {
    GphGaObject *o;
    s32 i;
    u16 n;
    u32 size;
    u8 *buf;

    n = g->gaObjCount;
    o = g->gaObjs;
    for (i = 0; i < n; i++) {
        size = ((o->vtxCount + 1) & 0xFFE) * 8;
        buf = GsPacketAlloc64(size + 0x10);
        o->deformBuf0 = buf;
        o->flags |= 2;
        copyq(buf, o->normals);
        buf += 16;
        MemMove128(buf, arg1, size >> 4);
        arg1 += size;
        o++;
    }
}
