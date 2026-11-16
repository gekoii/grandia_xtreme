#include "common.h"

f32 MapSplineSegIntegral(s32 h, f32 *y, s32 unused, f32 *m, s32 x) {
    s32 k = (x + h) / h;

    return h * ((y[k] + y[k - 1]) - (h * h) * (m[k - 1] + m[k]) * 0.083333336f) * 0.5f;
}
