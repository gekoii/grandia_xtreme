#include "common.h"

typedef f32 FVEC[4] __attribute__((aligned(16)));
typedef struct {
    FVEC pos[60];
    s32 tag[60];
    s32 head;
} Ring;

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void ChrTrailPush(Ring *arg0, FVEC *arg1, s32 arg2) {
    FVEC d;
    s32 i = arg0->head;
    s32 old = i;
    FVEC *p;

    i--;

    if (i <= -1) {
        i = 59;
    }
    p = (FVEC *)((u8 *)arg0 + (i << 4));
    copyq(p, arg1);
    arg0->tag[i] = arg2;
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyzw $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(d), "r"(p), "r"(arg0->pos[old]) : "memory");
    (*p)[3] = fsqrt(d[0] * d[0] + d[2] * d[2]);
    arg0->head = i;
}
