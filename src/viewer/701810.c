/* cflags: -ffast-math */
/* -ffast-math: sqrtf becomes a bare sqrt.s (no c.eq.s / jal sqrtf fallback) */
#include "common.h"
#include "viewer/n35.h"
#include "protos/viewer.h"

f32 VwrVecDistance(VuVec3 *a, VuVec3 *b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}
