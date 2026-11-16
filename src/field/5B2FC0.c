/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
} Parent;

typedef struct {
    u8 pad0[7];
    u8 flags;
    u8 pad8[4];
    Parent *parent;
    u8 pad10[0x10];
    FVECTOR pos;
    u8 pad30[0x20];
    f32 sx;
    f32 sy;
    u8 pad58[0x68];
    FVECTOR ofs;
    f32 rad;
    f32 h;
} Evt;

f32 VuVecDot(f32 *, f32 *);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define VADD(d, a, b) \
    __asm__ volatile("lqc2 $vf1, 0(%1)\n\tlqc2 $vf2, 0(%2)\n\tvadd.xyz $vf1, $vf1, $vf2\n\tsqc2 $vf1, 0(%0)" \
                     : : "r"(d), "r"(a), "r"(b) : "memory")
#define VSUB(d, a, b) \
    __asm__ volatile("lqc2 $vf1, 0(%1)\n\tlqc2 $vf2, 0(%2)\n\tvsub.xyz $vf1, $vf1, $vf2\n\tsqc2 $vf1, 0(%0)" \
                     : : "r"(d), "r"(a), "r"(b) : "memory")

s32 EvtAreaHitCylinder(f32 *m, f32 r, Evt *e) {
    FVECTOR d;
    FVECTOR w;
    FVECTOR q;
    FVECTOR c;
    FVECTOR top;
    f32 h;
    f32 rad;
    f32 inv;
    f32 t;

    h = e->h;
    rad = e->rad;
    if (e->parent) {
        VADD(c, e->pos, e->parent->pos);
    } else {
        copyq(c, e->pos);
    }
    VADD(c, c, e->ofs);
    copyq(top, c);
    if (e->flags & 8) {
        rad *= e->sx;
        h *= e->sy;
    }
    top[1] += h;
    VSUB(d, top, c);
    VSUB(w, m, c);
    h = fsqrt(VuVecDot(d, d));
    inv = 1.0f / h;
    t = VuVecDot(d, w) * inv;
    if (h < t) {
        return 0;
    }
    if (t < 0.0f) {
        return 0;
    }
    {
        f32 k = t * inv;
        d[0] *= k;
        d[1] *= k;
        d[2] *= k;
    }
    VSUB(q, w, d);
    r += rad;
    r *= r;
    if (r < VuVecDot(q, q)) {
        return 0;
    }
    return 1;
}
