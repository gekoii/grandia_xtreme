/* cflags: -ffast-math */
/* -ffast-math: sqrtf is a bare sqrt.s (no NaN check and sqrtf fallback call) */
#include "common.h"
#include "protos/field.h"


f32 FldVecDistXZ(f32 *a, f32 *b) {
    f32 dx = fabsf(a[0] - b[0]);
    f32 dz = fabsf(a[2] - b[2]);
    return sqrtf(dx * dx + dz * dz);
}
