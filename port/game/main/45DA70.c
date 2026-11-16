/* Port-only C for VuVecDot (0x0045DA70), which the decompilation still has in assembly. */
/*
 * Also VuVecNormalize (0x0045DA98), VuVecNormalizeLen (0x0045DAF8),
 * VuVecLerp (0x0045DB60), VuMtxMul (0x0045DBF0) and VuMtxInverse
 * (0x0045DE40), the small VU0 vector/matrix routines of the same asm file.
 *
 * VU0 registers: these leave their operands and results in $vf1-$vf9 on the
 * PS2, but no caller (C inline asm or assembly) reads them afterwards, so the
 * port doesn't write grx_vf. Vectors are whole quadwords (lqc2/sqc2): a
 * normalise or lerp writes w too (copied from the source).
 */
#include "common.h"
#include "vu0_port.h"

f32 VuVecDot(f32 *a, f32 *b);
void VuVecNormalize(f32 *dst, f32 *src);
f32 VuVecNormalizeLen(f32 *dst, f32 *src);
void VuVecLerp(f32 *dst, f32 *a, f32 *b, f32 t);
void VuMtxMul(f32 *dst, f32 *a, f32 *b);
void VuMtxInverse(f32 *dst, f32 *src);

/* vmul.xyz, vaddy.x, vaddz.x */
static f32 Dot3(const f32 *a, const f32 *b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

f32 VuVecDot(f32 *a, f32 *b) {
    return Dot3(a, b);
}

void VuVecNormalize(f32 *dst, f32 *src) {
    f32 v[4];
    f32 q;

    VuCopy4(v, src);
    q = VuRsqrt(Dot3(v, v));
    v[0] *= q;
    v[1] *= q;
    v[2] *= q;
    VuCopy4(dst, v);
}

/* Returns the length (1 / the rsqrt) */
f32 VuVecNormalizeLen(f32 *dst, f32 *src) {
    f32 v[4];
    f32 q;

    VuCopy4(v, src);
    q = VuRsqrt(Dot3(v, v));
    v[0] *= q;
    v[1] *= q;
    v[2] *= q;
    VuCopy4(dst, v);
    return VuDiv(1.0f, q);
}

/* dst.xyz = a + (b - a) * t, dst.w = a.w */
void VuVecLerp(f32 *dst, f32 *a, f32 *b, f32 t) {
    f32 v[4];
    s32 i;

    VuCopy4(v, a);
    for (i = 0; i < 3; i++) {
        v[i] = a[i] + (b[i] - a[i]) * t;
    }
    VuCopy4(dst, v);
}

/* dst[i] = b[i].x * a[0] + b[i].y * a[1] + b[i].z * a[2] + b[i].w * a[3]
 * (row vectors: dst = b * a). Everything is read before dst is written, so
 * dst may be a or b. */
void VuMtxMul(f32 *dst, f32 *a, f32 *b) {
    f32 m[16];
    s32 i;

    for (i = 0; i < 4; i++) {
        VuApplyMtx(&m[i * 4], &b[i * 4], a);
    }
    VuCopyMtx(dst, m);
}

/* Inverse of a rotation + translation: the transposed 3x3 (w = 0), then
 * row 3 = (-(r0.t), -(r1.t), -(r2.t), t.w) */
void VuMtxInverse(f32 *dst, f32 *src) {
    f32 m[16];
    f32 t[4];
    s32 i;

    VuCopy4(t, &src[12]);
    for (i = 0; i < 3; i++) {
        m[i * 4 + 0] = src[0 + i];
        m[i * 4 + 1] = src[4 + i];
        m[i * 4 + 2] = src[8 + i];
        m[i * 4 + 3] = 0.0f;
    }
    for (i = 0; i < 3; i++) {
        m[12 + i] = 0.0f - (src[i * 4 + 0] * t[0] + src[i * 4 + 1] * t[1] + src[i * 4 + 2] * t[2]);
    }
    m[15] = t[3];
    VuCopyMtx(dst, m);
}
