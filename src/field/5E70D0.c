/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n17.h"

typedef struct {
    s32 unk0;
    u8 pad4[0x8];
    f32 unkC;
    FVECTOR unk10;
    FVECTOR unk20;
    FVECTOR unk30;
    FVECTOR unk40;
} Obj;

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);

#define VSUB(out, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vsub.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(out), "r"(a), "r"(b) : "memory")

#define VADD(out, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(out), "r"(a), "r"(b) : "memory")

#define VSCALE(out, a, s) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "qmtc2.ni %2, $vf2\n\t" \
    "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(out), "r"(a), "r"(s) : "memory")

/* The two do { } while (0) blocks (probably left by the original's macros)
 * are needed: plain if/return code differs in the branch layout, the
 * alignment nop, the copy of a's address and register allocation. */
s32 MapPolySweepSlide(MapPolyTri *p, Obj *o)
{
    FVECTOR t;
    FVECTOR a;
    FVECTOR b;
    FVECTOR c;
    FVECTOR d;
    FVECTOR e;
    FVECTOR f;
    FVECTOR n;
    f32 dist;
    f32 r;
    f32 dn;
    f32 k0;
    f32 k1;

    VSUB(d, o->unk10, p->v0);
    VuVecCopy(a, o->unk20);
    k0 = 0.0f;
    k1 = 1.0f;
    r = o->unkC;
    VuVecCopy(n, p->normal);
    do {
        dist = VuVecDot(d, n);
        if (dist < k0) {
            return 0;
        }
    } while (0);
    do {
        VADD(t, a, d);
        if (r < VuVecDot(t, n)) {
            goto fail;
        }
        dn = -VuVecDot(a, n);
        VSCALE(e, n, -dn);
    } while (0);
    VSUB(f, a, e);
    if (dist < r) {
        VSCALE(b, n, r - dist);
        VuVecCopy(c, f);
    } else {
        if (__builtin_fabsf(dn) > 0.01f) {
            k0 = (dist - r) / dn;
            k1 = k1 - k0;
        }
        VSCALE(b, a, k0);
        VSCALE(c, f, k1);
    }
    VuVecCopy(o->unk30, b);
    VuVecCopy(o->unk40, c);
    return 1;
fail:
    return 0;
}
