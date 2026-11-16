/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
} Base;

typedef struct {
    u8 pad0[0x7];
    u8 flags;
    u8 pad8[0xC - 0x8];
    Base *base;
    u8 pad10[0x20 - 0x10];
    FVECTOR pos;
    u8 pad30[0x40 - 0x30];
    FVECTOR ofs;
    FVECTOR scale;
    u8 pad60[0x80 - 0x60];
    FVECTOR mtx[4];
    FVECTOR size;
} Obj;

extern FVECTOR mapHtaBoxNormals[6];
extern FVECTOR mapHtaHalfVec;

void VuVecCopy(f32 *, f32 *);
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

#define VZERO(o) \
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(o) : "memory")

#define VOP(op, o, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        op " $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(a), "r"(b) : "memory")

#define VADD(o, a, b) VOP("vadd.xyz", o, a, b)
#define VSUB(o, a, b) VOP("vsub.xyz", o, a, b)
#define VMUL(o, a, b) VOP("vmul.xyz", o, a, b)

#define VAPPLY(o, m, v) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%2)\n\t" \
        "lqc2 $vf4, 0x0(%1)\n\t" \
        "lqc2 $vf5, 0x10(%1)\n\t" \
        "lqc2 $vf6, 0x20(%1)\n\t" \
        "lqc2 $vf7, 0x30(%1)\n\t" \
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(o), "r"(m), "r"(v) : "memory")

s32 MapHtaHitBox(f32 *a, f32 *b, Obj *o, f32 h)
{
    FVECTOR t;
    FVECTOR d;
    FVECTOR q;
    FVECTOR n;
    FVECTOR p;
    FVECTOR bn;
    FVECTOR r0;
    FVECTOR r1;
    FVECTOR e;
    FVECTOR c;
    FVECTOR ext;
    s32 i;
    s32 ground;
    s32 hit;
    f32 gy;
    f32 y;
    f32 best;
    f32 bd;
    f32 bv;
    f32 dn;
    f32 vn;
    f32 k;
    f32 zero;
    f32 s;

    if (fsqrt(VuVecDot(b, b)) < 1.0f) {
        VZERO(b);
        return 0;
    }
    VMUL(t, o->size, mapHtaHalfVec);
    VMUL(ext, t, o->scale);
    if (!(o->flags & 4)) {
        if (o->base != NULL) {
            VADD(c, o->pos, o->base->pos);
        } else {
            VuVecCopy(c, o->pos);
        }
    }
    gy = 0.0f;
    hit = 0;
    ground = 0;
    best = 0.0f;
    bd = 0.0f;
    bv = 0.0f;
    for (i = 0; i < 6; i++) {
        if (o->flags & 4) {
            VMUL(p, ext, mapHtaBoxNormals[i]);
            VADD(p, p, o->ofs);
            VAPPLY(n, o->mtx, mapHtaBoxNormals[i]);
            VAPPLY(p, o->mtx, p);
        } else {
            copyq(n, mapHtaBoxNormals[i]);
            VMUL(p, ext, mapHtaBoxNormals[i]);
            VADD(p, p, c);
        }
        VuVecCopy(q, a);
        q[1] += h;
        VSUB(d, q, p);
        if (!(n[1] < 0.64278757f)) {
            y = p[1] - (n[0] * d[0] + n[2] * d[2]) / n[1] - q[1] + h;
            if (y < 0.0f) {
                if (y < b[1]) {
                    hit = 0;
                    goto skip; /* gotos: the original's block layout (if/else form differs) */
                }
                if (y <= 50.1f) {
                    gy = y;
                    goto set;
                }
            } else if (y <= 50.1f) {
                gy = y;
            set:
                ground = 1;
            }
        skip:;
        }
        dn = VuVecDot(d, n);
        vn = -VuVecDot(b, n);
        y = dn - vn;
        if (h < y) {
            hit = 0;
            break;
        }
        zero = 0.0f;
        if (y < zero) continue;
        if (zero < VuVecDot(b, n)) continue;
        if (__builtin_fabsf(vn) < 0.001f) continue;
        y = __builtin_fabsf(y);
        if (best < y) {
            VuVecCopy(bn, n);
            best = y;
            bd = dn;
            bv = vn;
            hit++;
        }
    }
    if (hit == 0) {
        return 0;
    }
    if (ground) {
        b[1] = gy;
        return 1;
    }
    if (bd < h) {
        t[0] = h - bd;
        t[1] = h - bd;
        t[2] = h - bd;
        VMUL(r0, bn, t);
        VuVecCopy(r1, b);
    } else {
        k = (bd - h) / bv;
        t[0] = k;
        t[1] = k;
        t[2] = k;
        VMUL(r0, b, t);
        t[0] = 1.0f - k;
        t[1] = 1.0f - k;
        t[2] = 1.0f - k;
        VMUL(r1, b, t);
    }
    t[0] = -bv;
    t[1] = -bv;
    t[2] = -bv;
    VMUL(bn, bn, t);
    VSUB(e, b, bn);
    y = e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    if (y < 0.01f) {
        copyq(b, r0);
        return 2;
    }
    s = 1.0f / fsqrt(y);
    t[0] = s;
    t[1] = s;
    t[2] = s;
    VMUL(e, e, t);
    s = VuVecDot(r1, e);
    t[0] = s;
    t[1] = s;
    t[2] = s;
    VMUL(e, e, t);
    if (!(bn[1] < 0.64278757f)) {
        e[1] = 0.0f;
    }
    VADD(b, r0, e);
    return 2;
}
