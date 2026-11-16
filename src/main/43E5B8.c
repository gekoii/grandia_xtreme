/* as: ee-gcc2.9-991111-01 */
#include "common.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void VuMatrixToQuat(f32 *q, f32 m[3][4]) {
    f32 ww;
    f32 a;
    f32 b;
    f32 c;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 w;
    f32 x;
    f32 y;
    f32 z;
    f32 d1;
    f32 d2;
    f32 d3;
    f32 e1;
    f32 e2;
    f32 e3;
    f32 e4;
    f32 e5;
    f32 e6;

    e1 = m[0][1];
    e2 = m[1][0];
    e3 = m[1][2];
    e4 = m[2][1];
    e5 = m[0][2];
    e6 = m[2][0];
    d1 = (e3 - e4) * 0.5f;
    d2 = (e6 - e5) * 0.5f;
    d3 = (e1 - e2) * 0.5f;
    a = (1.0f - m[1][1]) * 0.5f;
    b = (1.0f - m[0][0]) * 0.5f;
    c = (1.0f - m[2][2]) * 0.5f;
    x2 = (a - b + c) * 0.5f;
    y2 = (-a + b + c) * 0.5f;
    z2 = (a + b - c) * 0.5f;
    ww = 1.0f - (x2 + y2 + z2);
    x = fsqrt(x2);
    y = fsqrt(y2);
    z = fsqrt(z2);
    w = fsqrt(ww);
    if (d1 < 0.0f) {
        x = -x;
    }
    if (d2 < 0.0f) {
        y = -y;
    }
    if (d3 < 0.0f) {
        z = -z;
    }
    q[1] = x;
    q[2] = y;
    q[3] = z;
    q[0] = w;
}
