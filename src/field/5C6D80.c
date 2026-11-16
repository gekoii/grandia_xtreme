/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n14.h"
#include "protos/field.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 MapPathSplineResample(f32 **in, f32 **out, s32 n) {
    f32 v[3];
    f32 sum;
    f32 pos;
    s32 i;
    s32 j;
    s32 k;
    s32 nm1;
    s32 seg;
    s32 step;
    s32 cnt;
    s32 w;
    s32 o;

    sum = 0.0f;
    for (i = 0; i < n; i++) {
        sum += in[3][i];
    }
    nm1 = n - 1;
    seg = sum / n;
    if (seg == 0) {
        seg++;
    }
    step = 0;
    for (j = 0; j < nm1; j++) {
        v[0] = in[0][j + 1] - in[0][j];
        v[1] = in[1][j + 1] - in[1][j];
        v[2] = in[2][j + 1] - in[2][j];
        v[0] = fsqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if (step < v[0]) {
            step = v[0];
        }
    }
    step = step * 10 / seg;
    if (step == 0) {
        step = seg * 10;
    }
    for (i = 0; i < 3; i++) {
        MapSplineSolve(step, in[i], n, mapSplineSecondDeriv[i]);
    }
    MapSplineStepLengths(step, in, n, mapSplineSecondDeriv, 0);
    o = 0;
    for (i = 0; i < nm1; i++) {
        pos = 0.0f;
        w = 0;
        v[0] = 0.0f;
        for (j = 0; j < step; j++) {
            v[0] += mapSplineStepLengths[i * step + j];
        }
        cnt = v[0] / in[3][i];
        if (o + 3 > 0x660) {
            return o;
        }
        for (k = 0; k < 3; k++) {
            out[k][o / 3] = in[k][i];
            o++;
        }
        for (j = 1; j < cnt; j++) {
            if (o + 3 > 0x660) {
                return o;
            }
            if (w < step) {
                do {
                    f32 next = pos + mapSplineStepLengths[i * step + w];
                    f32 target = j * (v[0] / cnt);
                    if (__builtin_fabsf(target - __builtin_fabsf(pos)) < __builtin_fabsf(next - target)) {
                        break;
                    }
                    w++;
                    pos = next;
                } while (w < step);
            }
            for (k = 0; k < 3; k++) {
                out[k][o / 3] = MapSplineEval(step, in[k], n, mapSplineSecondDeriv[k], i * step + w);
                o++;
            }
        }
    }
    if (o + 3 > 0x660) {
        return o;
    }
    for (k = 0; k < 3; k++) {
        out[k][o / 3] = in[k][i];
        o++;
    }
    return o;
}
