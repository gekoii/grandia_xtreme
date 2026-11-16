#include "common.h"
#include "field/n14.h"
#include "protos/field.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void MapSplineStepLengths(s32 h, f32 **y, s32 n, f32 **m, s32 arg4) {
    f32 px, py, pz;
    f32 x, yy, z;
    f32 dx, dy, dz;
    f32 d;
    s32 i;

    px = MapSplineEval(h, y[0], n, m[0], 0);
    py = MapSplineEval(h, y[1], n, m[1], 0);
    pz = MapSplineEval(h, y[2], n, m[2], 0);
    for (i = 0; i < (n - 1) * h; i++) {
        if (i != (n - 1) * h - 1) {
            x = MapSplineEval(h, y[0], n, m[0], i + 1);
            yy = MapSplineEval(h, y[1], n, m[1], i + 1);
            z = MapSplineEval(h, y[2], n, m[2], i + 1);
        } else {
            x = y[0][n - 1];
            yy = y[1][n - 1];
            z = y[2][n - 1];
        }
        dx = x - px;
        dy = yy - py;
        dz = z - pz;
        d = fsqrt(dx * dx + dy * dy + dz * dz);
        mapSplineStepLengths[i] = d;
        px = x;
        py = yy;
        pz = z;
    }
}
