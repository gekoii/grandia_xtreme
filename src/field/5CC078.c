#include "common.h"
#include "field/n15.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void CamTrailReset(void *a, void *b) {
    camTrail->tail = 1;
    camTrail->head = 0;
    copyq(camTrail->points[0], a);
    copyq(camTrail->points[1], b);
}
