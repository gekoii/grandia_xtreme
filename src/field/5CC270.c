#include "common.h"
#include "field/n15.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

s32 CamTrailSetTail(s32 n, void *src) {
    s32 i;
    f32 *p;

    if (n < 0) {
        return 0;
    }
    i = camTrail->head + n;
    if (i >= 32) {
        i -= 32;
    }
    if (i < 0) {
        i += 32;
    }
    p = camTrail->points[i];
    copyq(p, src);
    camTrail->tail = i;
    return 1;
}
