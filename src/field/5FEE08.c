/* as: ee-gcc2.9-991111-01 */
#include "common.h"

f32 ChrAngleDiffDeg(f32 a, f32 b) {
    f32 d1 = b - a;
    f32 d0 = a - b;

    if (d1 >= 360.0f) {
        d1 -= 360.0f;
    }
    if (d1 < 0.0f) {
        d1 += 360.0f;
    }
    if (d0 >= 360.0f) {
        d0 -= 360.0f;
    }
    if (d0 < 0.0f) {
        d0 += 360.0f;
    }
    if (d1 == d0) {
        return d1;
    }
    if (d1 < d0) {
        return d1;
    }
    return -d0;
}
