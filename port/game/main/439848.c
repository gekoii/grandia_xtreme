/* Port-only C for GphGaObjDeformRipple (0x00439848), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "vu0_port.h"

/* Not protos/main.h: it declares VuCosf as returning s32 (from
 * src/main/45D648.c), but VuCosf returns a float ($f0, from VuSinAngle) */
f32 VuSinf(f32);
f32 VuCosf(f32);
void VuVecCopy(f32 *dst, f32 *src);
f32 VuVecDot(f32 *a, f32 *b);
void VuVecNormalize(f32 *dst, f32 *src);

/* The ripple: centre, radius where the amplitude reaches 0 (+0x10),
 * wavelength (+0x14), amplitude (+0x18), phase in degrees (+0x1C), and the
 * phase range drawn, in units of pi (+0x20..+0x24) */
typedef struct {
    /* 0x00 */ FVECTOR pos;
    /* 0x10 */ f32 radius;
    /* 0x14 */ f32 wavelength;
    /* 0x18 */ f32 amp;
    /* 0x1C */ f32 phase;
    /* 0x20 */ f32 phaseMin;
    /* 0x24 */ f32 phaseMax;
} Ripple;

void GphGaObjDeformRipple(GphGaObject *obj, Ripple *rp);

/*
 * Concentric sine wave around rp->pos, added to the vertex packet in place
 * (y += sin(phase - d * 2pi/wavelength - pi/2) * amplitude falling off
 * linearly with d; half of it, plus half the amplitude, in the first and
 * last pi of the range), and the vertex's 4.12 normal tilted to the slope.
 */
void GphGaObjDeformRipple(GphGaObject *obj, Ripple *rp) {
    FVECTOR center;
    f32 *pkt;
    s16 *nrm;
    f32 decay;
    f32 k;
    f32 lo;
    f32 hi;
    f32 phase;
    f32 amp;
    u32 n;
    u32 i;

    if (obj->vtxPkt == NULL || obj->deformBuf0 == NULL) {
        return;
    }
    obj->flags |= 2;
    center[3] = 0.0f;
    VuVecCopy(center, rp->pos);
    amp = rp->amp;
    decay = amp / rp->radius;
    k = VuBits(0x40C90FD8) / rp->wavelength; /* 2pi */
    nrm = (s16 *)((u8 *)obj->deformBuf0 + 0x10);
    lo = rp->phaseMin * VuBits(0x40490FD8); /* pi */
    hi = rp->phaseMax * VuBits(0x40490FD8);
    phase = rp->phase * VuBits(0x3C8EFA35); /* pi / 180 */
    n = obj->vtxCount;
    pkt = (f32 *)obj->vtxPkt;
    if (n == 0) {
        return;
    }
    for (i = 0; i < n; i++) {
        FVECTOR p;
        FVECTOR d;
        f32 dist;
        f32 a;
        f32 ph;
        f32 slope;
        s32 w[4];
        s32 c;

        VuCopy4(p, &pkt[i * 4]);
        VuCopy4(d, p);
        for (c = 0; c < 3; c++) {
            d[c] = p[c] - center[c];
        }
        d[1] = 0.0f;
        dist = VuSqrt(VuVecDot(d, d));
        a = amp - dist * decay;
        if (a < 0.0f) {
            continue;
        }
        ph = phase - dist * k;
        if (ph < lo) {
            continue;
        }
        if (hi < ph) {
            continue;
        }
        if (ph < lo + VuBits(0x40490FD8) || hi - VuBits(0x40490FD8) < ph) {
            p[1] = p[1] + (VuSinf(ph - VuBits(0x3FC90FD8)) * a * 0.5f + a * 0.5f);
        } else {
            p[1] = p[1] + VuSinf(ph - VuBits(0x3FC90FD8)) * a;
        }
        VuCopy4(&pkt[i * 4], p);
        slope = VuCosf(ph - VuBits(0x3FC90FD8)) * a * k;
        if (((slope < 0.0f) ? -slope : slope) < VuBits(0x3A83126F)) { /* 0.001 */
            continue;
        }
        if (slope < 0.0f) {
            d[0] = d[0] * -1.0f;
            d[1] = d[1] * -1.0f;
            d[2] = d[2] * -1.0f;
        }
        d[1] = 0.0f;
        slope = 1.0f / slope;
        slope = (slope < 0.0f) ? -slope : slope;
        VuVecNormalize(d, d);
        d[1] = slope;
        VuVecNormalize(d, d);
        for (c = 0; c < 4; c++) {
            w[c] = VuFtoi(d[c], 12);
        }
        VuPackH4(&nrm[i * 4], w);
    }
}
