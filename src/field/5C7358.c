/* cc: ee-gcc2.9-991111-01 */
#include "common.h"

f32 MapSplineEvalDeriv(s32 h, f32 *y, s32 unused, f32 *m, s32 x) {
    s32 k = (x + h) / h;
    f32 hh = h * h;
    s32 a = k * h - x;
    s32 b = x - (k - 1) * h;

    return (m[k - 1] * (hh - (a * a) * 3.0f) + m[k] * ((b * b) * 3.0f - hh) + (y[k] - y[k - 1]) * 6.0f) / (h * 6.0f);
}
