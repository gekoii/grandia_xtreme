/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

typedef f32 FVEC[4] __attribute__((aligned(16)));
typedef f32 FMTX[4][4] __attribute__((aligned(16)));

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 pad;
    f32 u;
    f32 v;
    u32 col;
    u32 pad1C;
} Vtx;

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define vu0_apply(out, m, in) \
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
        : : "r"(out), "r"(m), "r"(in) : "memory")

extern void VuMtxSetRotZ(FMTX *, s32);

void WmapWarpCalcGrid(f32 a, f32 cx, f32 cz, f32 period) {
    FVEC v;
    FMTX m;
    Vtx *base;
    Vtx *p;
    s32 i;
    s32 j;
    s32 r;
    s32 g;
    s32 b;
    f32 k;
    f32 fx;
    f32 fz;
    f32 dx;
    f32 dz;
    f32 d;
    f32 t;
    f32 s;
    f32 amp;
    f32 su;
    f32 sv;
    f32 sc;
    f32 w;
    f32 c20;

    base = WmapWarpGetGrid();
    k = 6.283184f / period;
    su = 0.0009765625f;
    sv = 0.0019508929f;
    sc = 0.01f;
    c20 = 20.0f;
    for (i = 0; i < 33; i++) {
        p = &base[i * 33];
        for (j = 0; j < 33; j++, p++) {
            fz = (f32)i * 14.0f;
            fx = (f32)j * c20;
            dz = fz - cz;
            dx = fx - cx;
            d = fsqrt(dx * dx + dz * dz);
            p->x = fx;
            p->y = fz;
            p->z = 100.0f;
            t = a - d * k;
            amp = VuSinf(t);
            s = amp * 0.5f + 0.5f;
            if (t < 0.0f) {
                amp = s = 0.0f;
            }
            b = g = r = 0x80;
            if (d > 0.5f) {
                w = amp * 0.05f;
                w = w * sinf(a * 0.01f - atan2f(dz, dx) * 8.0f);
                VuMtxSetRotZ(&m, (s32)(w * 10430.378f));
                v[0] = dx;
                v[1] = dz;
                v[3] = v[2] = 0.0f;
                vu0_apply(v, m, v);
                dx = v[0];
                dz = v[1];
                p->u = su * (cx + dx) + dx / d * s * sc;
                p->v = sv * (cz + dz) + dz / d * s * sc;
            } else {
                p->u = su * fx + s * sc;
                p->v = sv * fz - s * sc;
            }
            if (p->u > 0.625f) {
                p->u = 0.625f;
            }
            if (p->v > 0.874f) {
                p->v = 0.874f;
            }
            if (i == 0 || i == 32 || j == 0 || j == 32) {
                p->u = su * fx;
                p->v = sv * fz;
            }
            p->col = r | (b << 16) | 0x80000000 | (g << 8);
        }
    }
}
