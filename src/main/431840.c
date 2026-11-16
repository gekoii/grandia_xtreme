#include "common.h"

typedef int s128 __attribute__((mode(TI)));

typedef struct {
    f32 v[4];
} Vec4;

typedef struct {
    u8 pad[0x50];
    Vec4 a;
    Vec4 b;
    s128 d;
    u8 pad2[0x9C - 0x80];
    f32 len;
} Obj;

f32 VuVecNormalizeLen(Vec4 *, Vec4 *);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void CamCalcDir(Obj *o) {
    Vec4 t;
    f32 len;

    __asm__ volatile(
        "lqc2 $vf1, 0(%0)\n\t"
        "lqc2 $vf2, 0(%1)\n\t"
        "vsub.xyzw $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%2)"
        : : "r"(&o->a), "r"(&o->b), "r"(&t) : "memory");
    len = VuVecNormalizeLen(&t, &t);
    copyq(&o->d, &t);
    o->len = len;
}
