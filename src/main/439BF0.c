/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

typedef struct {
    f32 f[4];
} Vec4;

s32 fptoui(f32);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void GphGaObjDeformVortex(GphGaObject *o, f32 t, f32 b, f32 c) {
    u32 *out;
    u32 *p;
    u32 *hdr;
    s32 i;
    s32 hi;
    s32 n;
    f32 f;
    Vec4 *vin;
    Vec4 *vout;
    Vec4 v;
    f32 r;
    f32 x;
    f32 z;
    f32 h;
    f32 a;

    p = o->deformBuf1;
    if (p == NULL) {
        return;
    }
    out = p;
    n = o->vtxCount;
    hdr = o->uvs;
    i = n;
    o->flags |= 2;
    f = t * 0.05f;
    out[0] = hdr[0];
    out[1] = hdr[1];
    out[2] = hdr[2];
    out[3] = hdr[3];
    out += 4;
    vout = (Vec4 *)*(u32 *)&o->vtxPkt; /* read as u32 (alias set of the out[] stores) */
    vin = (Vec4 *)o->verts;
    while (f > 2.0f) {
        f -= 1.0f;
    }
    if (n != 0) do {
        copyq(&v, vin);
        x = v.f[0];
        z = v.f[2];
        r = fsqrt(x * x + z * z);
        z = z / r;
        x = x / r;
        h = (r + 300.0f - t) * 0.01f;
        if (z > 0.0f) {
            a = VuAcosf(x) / 3.141592f - r * 0.01f;
        } else {
            a = VuAcosf(x) / 3.141592f + r * 0.01f;
        }
        if (a > 1.0f) {
            a = 2.0f - a;
        }
        if (a < 0.0f) {
            a = -a;
        }
        if (r < 10.0f) {
            v.f[1] = -400.0f;
        } else {
            v.f[1] = r * 4.0f + -400.0f;
        }
        copyq(vout, &v);
        i--;
        vin++;
        vout++;
        hi = fptoui(h * 1024.0f) << 16;
        *out++ = hi | fptoui(a * 1024.0f);
    } while (i != 0);
    for (i = (u32)out & 3; i != 0; i--) {
        *out++ = 0;
    }
}
