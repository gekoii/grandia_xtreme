#include "common.h"
#include "protos/main.h"
#include "gph.h"

typedef int s128 __attribute__((mode(TI)));

typedef union {
    s128 q;
    f32 f[4];
} Vec4;

void GphGaObjDeformWave(GphGaObject *o, f32 angle, f32 amp, f32 freq) {
    u32 i;
    u32 n;
    Vec4 *dst;
    Vec4 *src;
    f32 base;
    f32 a;
    f32 b;
    Vec4 t;

    dst = (Vec4 *)o->vtxPkt;
    if (dst == NULL) {
        return;
    }
    n = o->vtxCount;
    src = (Vec4 *)o->verts;
    o->flags |= 2;
    i = 0;
    if (i < n) {
        base = angle * 0.017453292f;
        do {
            Vec4 *d = &dst[i];
            /* do-while(0): doubles the loop weight of the wave temporaries (allocation) */
            do {
                t.q = src[i].q;
                a = t.f[0] * freq;
                b = t.f[2] * freq * 0.5f;
                a = VuSinf(base + a) * amp;
                b = VuSinf(base + b) * amp;
                t.f[0] += a * 0.2f;
                t.f[1] = t.f[1] + a + b;
                t.f[2] += b * 0.2f;
            } while (0);
            d->q = t.q;
            i += 1;
        } while (i < n);
    }
}
