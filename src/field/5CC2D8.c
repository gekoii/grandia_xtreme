/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    s32 unk0;
    u8 pad4[8];
    f32 unkC;
    FVECTOR unk10;
    FVECTOR unk20;
    FVECTOR unk30;
    u8 pad40[0x40];
} Coll;

f32 VuVecDot(void *, void *);

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

#define VSUB(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

#define HIT(c, dir, from) { \
    Coll *_p = (c); \
    f32 _k = 10.0f; \
    f32 *_q = (dir); \
    copyq(_p->unk20, _q); \
    copyq(_p->unk30, _q); \
    copyq(_p->unk10, (from)); \
    _p->unkC = _k; \
    _p->unk0 = 0; \
    FldCollSweep(&_p->unk0); \
}

void CamTrailUpdate(f32 *pos, f32 *target)
{
    FVECTOR prev;
    FVECTOR d;
    Coll c;
    s32 i;
    s32 n;

    CamTrailGet(2, prev);
    VSUB(d, prev, pos);
    if (fsqrt(VuVecDot(d, d)) > 0.01f) {
        HIT(&c, d, pos);
        if (c.unk0 != 0) {
            CamTrailPushFront(pos);
        } else {
            CamTrailSetHead(0, pos);
        }
    }
    i = 0;
    n = CamTrailCount() + 1;
    /* goto loop: written as for/while it is 9 insns longer (98 words off) */
    goto loop;
inc:
    i++;
loop:
    if (i < n) {
        CamTrailGet(i, prev);
        VSUB(d, target, prev);
        {
            Coll *cp = &c;
            f32 *pv = prev;
            HIT(cp, d, pv);
        }
        if (c.unk0 != 0) goto inc;
        CamTrailSetTail(i + 1, target);
    }
}
