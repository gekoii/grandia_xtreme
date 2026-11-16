/* Port-only C for VuCardinalInterp (0x0043E0E8), which the decompilation still has in assembly. */
#include "common.h"
#include "vu.h"
#include "vu0_port.h"

extern FVECTOR vuHermiteBasis[4];

void VuCardinalInterp(f32 *out, f32 *p, f32 *tension, f32 t);

/*
 * Cardinal spline between p[1] and p[2] (p[0..3] quadwords): tangents
 * (p2 - p0) * k and (p3 - p1) * k with k = tension[0], Hermite weights
 * (t^3, t^2, t, 1) * vuHermiteBasis. All four lanes. The asm loads a whole
 * quadword at tension but uses only its x.
 */
void VuCardinalInterp(f32 *out, f32 *p, f32 *tension, f32 t) {
    f32 k = tension[0];
    f32 tp[4];
    f32 h[4];
    f32 r[4];
    s32 i;

    tp[0] = t * t * t;
    tp[1] = t * t;
    tp[2] = t;
    tp[3] = 1.0f;
    VuApplyMtx(h, tp, &vuHermiteBasis[0][0]);
    for (i = 0; i < 4; i++) {
        f32 m0 = (p[8 + i] - p[0 + i]) * k;
        f32 m1 = (p[12 + i] - p[4 + i]) * k;

        r[i] = p[4 + i] * h[0] + p[8 + i] * h[1] + m0 * h[2] + m1 * h[3];
    }
    VuCopy4(out, r);
}
