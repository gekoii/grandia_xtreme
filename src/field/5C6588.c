#include "common.h"
#include "field/n14.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}
void ChrTrailGetDelayed(ChrTrail *arg0, FVECTOR *arg1) {
    s32 n;

    n = arg0->head;
    n += 8;
    if (n >= 60) {
        n -= 60;
    }
    copyq(arg1, (u8 *)arg0 + (n << 4));
}
