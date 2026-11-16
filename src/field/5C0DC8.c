/* cflags: -ffast-math */
/* -ffast-math: sqrtf is a bare sqrt.s (no NaN check and sqrtf fallback call) */
#include "common.h"
#include "protos/field.h"


f32 FldVecDist(f32 *a, f32 *b) {
    f32 dx = a[0] - b[0];
    f32 dy = a[1] - b[1];
    f32 dz = a[2] - b[2];
    return sqrtf(dx * dx + dy * dy + dz * dz);
}
