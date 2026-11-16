#include "common.h"
#include "field/n14.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}
f32 ChrTrailSumDist(ChrTrail *arg0, s32 arg1, s32 arg2) {
    f32 sum;
    s32 b, i;

    if (arg1 == arg2) {
        return 0.0f;
    }
    arg1 += arg0->head;
    arg2 += arg0->head;
    arg1 %= 60;
    b = arg2 % 60;
    sum = 0.0f;
    if (b < arg1) {
        for (i = arg1; i < 60; i++) {
            sum += arg0->pos[i][3];
        }
        arg1 = 0;
    }
    for (i = arg1; i < b; i++) {
        sum += arg0->pos[i][3];
    }
    return sum;
}
