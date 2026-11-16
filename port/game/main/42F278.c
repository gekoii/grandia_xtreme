/* Port-only C for VuBezier3 (0x0042F278), which the decompilation still has in assembly. */
#include "common.h"
#include "vu.h"
#include "vu0_port.h"

extern const FVECTOR vuBezierCoef; /* (3, 3, 3, 1) */

void VuBezier3(f32 *out, f32 *p, f32 t);

/*
 * Cubic Bezier through p[0..3] at t: out.xyz = p0 s^3 + c p1 t s^2 +
 * c p2 t^2 s + p3 t^3 with s = 1 - t and c = vuBezierCoef (per component),
 * multiplied in the asm's order. out.w = p[0].w (the vector ops are .xyz).
 */
void VuBezier3(f32 *out, f32 *p, f32 t) {
    f32 c[4];
    f32 q[4][4];
    f32 r[4];
    f32 s = 1.0f - t;
    s32 i;

    VuCopy4(c, vuBezierCoef);
    VuCopy4(q[0], &p[0]);
    VuCopy4(q[1], &p[4]);
    VuCopy4(q[2], &p[8]);
    VuCopy4(q[3], &p[12]);
    for (i = 0; i < 3; i++) {
        q[0][i] = q[0][i] * s;
        q[0][i] = q[0][i] * s;
        q[0][i] = q[0][i] * s;
        q[1][i] = c[i] * q[1][i];
        q[1][i] = q[1][i] * t;
        q[1][i] = q[1][i] * s;
        q[1][i] = q[1][i] * s;
        q[2][i] = c[i] * q[2][i];
        q[2][i] = q[2][i] * t;
        q[2][i] = q[2][i] * t;
        q[2][i] = q[2][i] * s;
        q[3][i] = q[3][i] * t;
        q[3][i] = q[3][i] * t;
        q[3][i] = q[3][i] * t;
    }
    VuCopy4(r, q[0]);
    for (i = 0; i < 3; i++) {
        r[i] = q[0][i] + q[1][i];
        r[i] = r[i] + q[2][i];
        r[i] = r[i] + q[3][i];
    }
    VuCopy4(out, r);
}
