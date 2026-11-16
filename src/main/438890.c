#include "common.h"
#include "protos/main.h"
#include "gph.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

typedef struct {
    f32 f[4];
} __attribute__((aligned(16))) Vec4;



void GphGaObjDeformSway(GphGaObject *o, f32 angle, f32 amp, f32 freq) {
    u8 *dst;
    u8 *src;
    u32 n;
    u32 i;
    u32 off;
    u32 off2;
    f32 base;
    f32 xa;
    f32 za;
    f32 t;
    Vec4 v[2];


    dst = o->deformBuf0;
    if (dst == NULL) {
        return;
    }
    n = o->vtxCount;
    if (o->skin == NULL) {
        src = (u8 *)o->verts;
        o->deformBuf1 = o->normals;
    } else {
        src = dst;
    }
    for (i = 0; i < n; i += 2) {
        off = i * 16;
        copyq(&v[0], src + off);
        off2 = off + 16;
        copyq(&v[1], src + off2);
        base = angle * 0.017453292f;
        xa = v[0].f[0] * freq;
        za = v[0].f[2] * freq * 0.5f;
        t = VuSinf(base + xa) * amp;
        v[0].f[0] += t;
        t = VuSinf(base + za) * amp;
        v[0].f[2] += t;
        xa = v[1].f[0] * freq;
        za = v[1].f[2] * freq * 0.5f;
        t = VuSinf(base + xa) * amp;
        v[1].f[0] += t;
        t = VuSinf(base + za) * amp;
        v[1].f[2] += t;
        copyq(dst + off, &v[0]);
        copyq(dst + off2, &v[1]);
    }
}
