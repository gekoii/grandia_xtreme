/* Port-only C for GphMotQuatSlerp (0x00435530), which the decompilation
   still has in assembly (asm/main/main/0352B8.s). Its caller is
   GphMotKeyLerpQuat (port/game/main/435468.c). */
#include "common.h"

f32 VuAcosf(f32);
f32 VuSinf(f32);

void GphMotQuatSlerp(f32 *out, f32 *a, f32 *b, f32 t);

#define SLERP_LERP_COS 0x3F7851ECu /* 0.97 */

/*
 * out = a * wa + b * wb. With d = a . b (summed as (x + y) + (z + w)),
 * negated with t when negative (wa uses the original t): above cos 0.97
 * a plain lerp (wa = 1 - t, wb = t), otherwise with th = acos(min(d, 1)),
 * wa = sin(th (1 - t)) / sin th and wb = sin(th t) / sin th.
 */
void GphMotQuatSlerp(f32 *out, f32 *a, f32 *b, f32 t) {
    union {
        f32 f;
        u32 u;
    } lim;
    f32 p[4];
    f32 r[4];
    f32 d;
    f32 wa, wb;
    f32 th, inv;
    s32 i;

    for (i = 0; i < 4; i++) {
        p[i] = a[i] * b[i];
    }
    d = (p[0] + p[1]) + (p[2] + p[3]);
    wa = 1.0f - t;
    wb = t;
    if (d < 0.0f) {
        d = -d;
        wb = -t;
    }
    lim.u = SLERP_LERP_COS;
    if (d < lim.f) {
        th = VuAcosf(d < 1.0f ? d : 1.0f);
        inv = 1.0f / VuSinf(th);
        wa = VuSinf(th * wa) * inv;
        wb = VuSinf(th * wb) * inv;
    }
    for (i = 0; i < 4; i++) {
        r[i] = a[i] * wa + b[i] * wb;
    }
    __builtin_memcpy(out, r, sizeof(r));
}
