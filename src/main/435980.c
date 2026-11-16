#include "common.h"
#include "gph.h"
#include "protos/main.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void GphMorphPositionsCopy(GphPart *obj, u8 *src) {
    GphGaObject *e;
    u8 *dst;
    s32 i;
    s32 j;
    s32 n;
    s32 cnt;

    n = obj->gaObjCount;
    e = obj->gaObjs;
    for (i = 0; i < n; e++, i++) {
        cnt = e->vtxCount;
        dst = GsPacketAlloc64(cnt * 16);
        e->vtxPkt = dst;
        e->flags |= 2;
        for (j = 0; j < cnt; j++) {
            copyq(dst, src);
            dst += 16;
            src += 16;
        }
    }
}
