/*
 * Helpers for the port-only C of the main executable's VU0 routines
 * (the .c files here). The routines are written as plain float C; these
 * cover what the VU0 does differently from IEEE floats where it matters:
 * vftoi saturates, and VDIV/VRSQRT give +-max instead of Inf on a zero
 * divisor (so normalising a zero vector gives 0, not NaN).
 */
#ifndef PORT_VU0_PORT_H
#define PORT_VU0_PORT_H

#include "common.h"

#define VU_FMAX 3.4028235e38f

typedef union {
    f32 f;
    s32 i;
    u32 u;
} VuWord;

/* A float constant by its bits, as the asm loads it (lui/ori) */
static inline f32 VuBits(u32 bits) {
    VuWord w;

    w.u = bits;
    return w.f;
}

/* vftoi0/4/12/15: truncate x * 2^frac towards zero, saturating */
static inline s32 VuFtoi(f32 x, s32 frac) {
    f32 y = x * (f32)(1 << frac);

    if (y >= 2147483647.0f) {
        return 0x7FFFFFFF;
    }
    if (y <= -2147483648.0f) {
        return (s32)0x80000000;
    }
    return (s32)y;
}

/* vitof0/4/12/15 */
static inline f32 VuItof(s32 i, s32 frac) {
    return (f32)i / (f32)(1 << frac);
}

/* VDIV Q, a, b */
static inline f32 VuDiv(f32 a, f32 b) {
    if (b == 0.0f) {
        VuWord wa, wb;

        wa.f = a;
        wb.f = b;
        return ((wa.u ^ wb.u) & 0x80000000) ? -VU_FMAX : VU_FMAX;
    } else {
        /* The VU flushes denormal results to 0 (1 / max, say) */
        f32 r = a / b;

        if (r > -1.17549435e-38f && r < 1.17549435e-38f) {
            r = 0.0f;
        }
        return r;
    }
}

/* VRSQRT Q, 1, x (the VU takes |x|) */
static inline f32 VuRsqrt(f32 x) {
    if (x < 0.0f) {
        x = -x;
    }
    if (x == 0.0f) {
        return VU_FMAX;
    }
    return 1.0f / __builtin_sqrtf(x);
}

/* EE sqrt.s (also takes |x|) */
static inline f32 VuSqrt(f32 x) {
    return __builtin_sqrtf(x < 0.0f ? -x : x);
}

static inline void VuCopy4(void *dst, const void *src) {
    __builtin_memcpy(dst, src, 16);
}

static inline void VuCopyMtx(void *dst, const void *src) {
    __builtin_memcpy(dst, src, 64);
}

/* vmulax/vmadday/vmaddaz/vmaddw with the rows of m: out = v.x * m[0] +
 * v.y * m[1] + v.z * m[2] + v.w * m[3] (out may alias v or m) */
static inline void VuApplyMtx(f32 *out, const f32 *v, const f32 *m) {
    f32 r[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        r[i] = m[i] * v[0] + m[4 + i] * v[1] + m[8 + i] * v[2] + m[12 + i] * v[3];
    }
    for (i = 0; i < 4; i++) {
        out[i] = r[i];
    }
}

/* The identity the routines build with vmr32 from $vf0 */
static inline void VuUnitMtx(f32 *m) {
    s32 i;

    for (i = 0; i < 16; i++) {
        m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

/* ppach of four words: their low halves */
static inline void VuPackH4(s16 *out, const s32 *w) {
    out[0] = (s16)w[0];
    out[1] = (s16)w[1];
    out[2] = (s16)w[2];
    out[3] = (s16)w[3];
}

#endif
