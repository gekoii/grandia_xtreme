/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

f32 VuVecDot(f32 *, f32 *);

#define COPYQ(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

#define VSUBW(o, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vsub.xyzw $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(o), "r"(a), "r"(b) : "memory")

#define VADDW(o, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyzw $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(o), "r"(a), "r"(b) : "memory")

#define VSCALEW(o, a, s) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "qmtc2.ni %2, $vf2\n\t" \
    "vmulx.xyzw $vf1, $vf1, $vf2x\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(o), "r"(a), "r"(s) : "memory")

s32 VuSphereSweepPlane2(FVECTOR *a, FVECTOR *out, FVECTOR *b, f32 lim)
{
    f32 *va = (f32 *)0x70003000;
    f32 *vb = (f32 *)0x70003010;
    f32 *vc = (f32 *)0x70003020;
    f32 *ve = (f32 *)0x70003040;
    f32 *vf = (f32 *)0x70003050;
    f32 *vg = (f32 *)0x70003060;
    f32 *vh = (f32 *)0x70003070;
    f32 *vi = (f32 *)0x70003080;
    f32 *vj = (f32 *)0x70003090;
    f32 *vk = (f32 *)0x700030A0;
    f32 t;
    f32 d;
    f32 q;
    f32 r;

    COPYQ(va, b[0]);
    COPYQ(vb, a[0]);
    COPYQ(vc, a[1]);
    COPYQ(ve, b[3]);
    VSUBW(vb, vb, va);
    t = -VuVecDot((f32 *)0x70003020, (f32 *)0x70003040);
    if (t < 0.0f) {
        return 0;
    }
    d = VuVecDot((f32 *)0x70003010, (f32 *)0x70003040);
    if (d < 0.0f) {
        return 0;
    }
    if (lim < d) {
        d = d - lim;
        q = d / t;
        VSCALEW(vf, vc, q);
        r = 1.0f - q;
        VSCALEW(vg, vc, r);
        VSCALEW(vk, ve, t * r);
        VADDW(vg, vg, vk);
        COPYQ(vh, vk);
        VSCALEW(vj, ve, t);
        VADDW(vi, vc, vj);
        vc = (f32 *)out;
        COPYQ(vc, vf);
        vc += 4;
        COPYQ(vc, vg);
        vc += 4;
        COPYQ(vc, vh);
        vc += 4;
        COPYQ(vc, vi);
        vc += 4;
        COPYQ(vc, vj);
    } else {
        VSCALEW(vg, ve, t);
        VADDW(vh, vg, vc);
        VSCALEW(vj, ve, t);
        VADDW(vi, vc, vj);
        vc = (f32 *)out;
        vc[0] = 0.0f;
        vc[1] = 0.0f;
        vc[2] = 0.0f;
        vc[3] = 0.0f;
        vc += 4;
        COPYQ(vc, vg);
        vc += 4;
        COPYQ(vc, vh);
        vc += 4;
        COPYQ(vc, vi);
        vc += 4;
        COPYQ(vc, vj);
    }
    return 1;
}
