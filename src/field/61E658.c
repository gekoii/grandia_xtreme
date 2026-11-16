/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR p0;
    FVECTOR e1;
    FVECTOR e2;
    FVECTOR n;
} Tri;

typedef struct {
    s32 unk0;
    u8 pad4[0x8];
    f32 unkC;
    FVECTOR pos;
    FVECTOR dir;
    FVECTOR out;
} Obj;

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);

static inline f32 fsqrt(f32 x)
{
    f32 r;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

#define VSUB(d, a, b) __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VADD(d, a, b) __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VSCALE(d, a, k) __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(k) : "memory")

#define VCROSS(d, a, b) __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vopmula.xyz ACC, $vf1, $vf2\n\t" \
        "vopmsub.xyz $vf1, $vf2, $vf1\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

s32 FldCollSphereTriTest(Tri *tri, Obj *o)
{
    FVECTOR n;
    FVECTOR d;
    FVECTOR dir;
    FVECTOR tmp;
    FVECTOR hit;
    FVECTOR c;
    FVECTOR w;
    FVECTOR res;
    FVECTOR e1;
    FVECTOR e2;
    FVECTOR dn;
    f32 r;
    f32 zero;
    f32 a;
    f32 h;
    f32 q;
    f32 dd;
    f32 k;
    f32 t;

    r = o->unkC;
    VuVecCopy(n, tri->n);
    VuVecCopy(e1, tri->e1);
    VuVecCopy(e2, tri->e2);
    VuVecCopy(dir, o->dir);
    VSUB(d, o->pos, tri->p0);
    a = VuVecDot(n, d);
    zero = 0.0f;
    if (a < zero) {
        return 0;
    }
    dd = -VuVecDot(n, dir);
    if (dd < zero) {
        return 0;
    }
    if (fabsf(dd) < 0.001f) {
        return 0;
    }
    h = a - r;
    if (dd < h) {
        return 0;
    }
    do { /* do-while(0): doubles the block's refs; the saved-register allocation depends on it */
        VADD(c, e1, e2);
        VSCALE(c, c, 0.33333334f);
        VSUB(w, tri->p0, o->pos);
        VADD(w, c, w);
        VuVecNormalize(dn, dir);
        t = VuVecDot(w, dn);
        q = VuVecDot(w, w) - t * t;
        a = VuVecDot(c, c);
        VSUB(w, e1, c);
        k = VuVecDot(w, w);
        if (a < k) {
            a = k;
        }
        VSUB(w, e2, c);
        k = VuVecDot(w, w);
        if (a < k) {
            a = k;
        }
        a = fsqrt(a);
        a = a + r;
        a = a * a;
        if (a < q) {
            return 0;
        }
    } while (0);
    k = h / dd;
    if (k < zero) {
        return 0;
    }
    VSCALE(tmp, dir, k);
    VADD(hit, tmp, d);
    VuVecCopy(res, tmp);
    VuVecCopy(c, tmp);
    VuVecCopy(w, tmp);
    VCROSS(n, tri->n, tri->e1);
    VuVecNormalize(n, n);
    VuVecCopy(tmp, hit);
    h = VuVecDot(n, tmp);
    if (h > zero && r < h) {
        return 0;
    }
    VCROSS(n, tri->e2, tri->n);
    VuVecNormalize(n, n);
    VuVecCopy(tmp, hit);
    h = VuVecDot(n, tmp);
    if (h > 0.0f && r < h) {
        return 0;
    }
    VuVecCopy(c, tri->n);
    VSUB(w, tri->e2, tri->e1);
    VCROSS(n, c, w);
    VuVecNormalize(n, n);
    VSUB(tmp, hit, tri->e1);
    h = VuVecDot(n, tmp);
    if (h > 0.0f && r < h) {
        return 0;
    }
    VuVecCopy(o->out, res);
    VuVecCopy(o->dir, res);
    o->unk0 = 1;
    return 1;
}
