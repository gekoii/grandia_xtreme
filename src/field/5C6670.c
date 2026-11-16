#include "common.h"
#include "field/n14.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void ChrTrailFill(ChrTrail *arg0, FVECTOR *arg1, s32 arg2) {
    s32 i;
    for (i = 0; i < 60; i++) {
        copyq(&arg0->pos[i], arg1);
        arg0->tag[i] = arg2;
    }
    arg0->head = 0;
}
