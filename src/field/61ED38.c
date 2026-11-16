/* as: ee-gcc2.9-991111-01 */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    s32 unk0;
    u8 pad4[0x8];
    f32 unkC;
    FVECTOR pos;
    FVECTOR dir;
    FVECTOR out;
} Obj;

f32 VuVecNormalizeLen(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);

static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

s32 FldCollSpherePointTest(f32 *p, Obj *o, f32 r)
{
    FVECTOR d;
    FVECTOR n;
    FVECTOR t;
    FVECTOR u;
    f32 rad;
    f32 k;
    f32 len;

    rad = r + o->unkC;
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(d), "r"(o->pos), "r"(p) : "memory");
    VuVecNormalizeLen(n, o->dir);
    k = -VuVecDot(n, d);
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "qmtc2.ni %2, $vf2\n\t"
        "vmulx.xyzw $vf1, $vf1, $vf2x\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(t), "r"(n), "r"(k) : "memory");
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vadd.xyz $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(u), "r"(d), "r"(t) : "memory");
    len = fsqrt(VuVecDot(u, u));
    if (rad < len) {
        return 0;
    }
    k = k - fsqrt(rad * rad + len * len);
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "qmtc2.ni %2, $vf2\n\t"
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(u), "r"(n), "r"(k) : "memory");
    copyq(o->out, u);
    o->unk0 = 1;
    return 1;
}
