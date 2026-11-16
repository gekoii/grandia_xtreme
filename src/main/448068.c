/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    s32 pad;
    f32 u;
    f32 v;
    u32 color;
    s32 pad2;
} WaveVtx;

extern WaveVtx (*gphRippleGrid)[33];

extern s32 fptoui(f32);

void GphFbRippleBuild(f32 phase, f32 cx, f32 cy, f32 wavelen, f32 range) {
    s32 i;
    s32 j;
    f32 k;
    f32 fx;
    f32 fy;
    f32 dx;
    f32 dy;
    f32 d;
    f32 a;
    f32 s;
    f32 t;
    f32 u;
    f32 w;
    f32 e;
    s32 c;
    WaveVtx *v;
    f32 su;
    f32 sv;
    f32 hf;
    f32 q5;
    f32 tw;

    k = 6.28318405f / wavelen;
    tw = 20.0f;
    su = 0.0009765625f;
    sv = 0.00195089285f;
    q5 = 0.005f;
    hf = 0.5f;
    for (i = 0; i < 33; i++) {
        for (j = 0; j < 33; j++) {
            fy = i * 14.0f;
            dy = fy - cy;
            fx = j * tw;
            v = &gphRippleGrid[i][j];
            dx = fx - cx;
            d = fsqrt(dx * dx + dy * dy);
            v->x = fx;
            v->y = fy;
            v->z = 100.0f;
            a = phase - d * k;
            s = VuSinf(a);
            t = s * hf + hf;
            if (a < 0.0f) {
                t = 0.0f;
                s = t;
            }
            if (range * 3.14159203f < a) {
                t = 0.0f;
                s = 0.0f;
            }
            c = fptoui(s * 32.0f + 128.0f);
            if (d > hf) {
                u = su * fx;
                w = sv * fy;
                v->v = w + (dy / d) * (t * q5 + t * 0.01f);
                v->u = u + (dx / d) * t * q5;
            } else {
                e = t * q5;
                u = su * fx;
                w = sv * fy;
                v->u = u + e;
                v->v = w - e;
            }
            if (i == 0 || i == 32 || j == 0 || j == 32) {
                v->u = u;
                v->v = w;
            }
            v->color = c | (0x80000000 | (c << 16) | (c << 8));
        }
    }
}
