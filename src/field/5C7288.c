/* cc: ee-gcc2.9-991111-01 */
#include "common.h"

f32 MapSplineEval(s32 h, f32 *y, s32 unused, f32 *m, s32 x) {
    s32 k = (x + h) / h;
    f32 hh = h * h;
    f32 a = k * h - x;
    f32 b = x - (k - 1) * h;

    return (m[k - 1] * a * a * a + m[k] * b * b * b + (y[k - 1] * 6.0f - hh * m[k - 1]) * a +
            (y[k] * 6.0f - hh * m[k]) * b) / (h * 6.0f);
}
