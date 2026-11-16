/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n33.h"
#include "party.h"

void VuVecNormalize(VuVec3 *, VuVec3 *);
static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlUnitMoveClamped(BtlUnit *u, f32 dx, f32 dz) {
    VuVec3 v;
    f32 x, z;
    f32 d;
    f32 k;

    x = u->pos[0] + dx;
    z = u->pos[2] + dz;
    u->pos[0] = x;
    u->pos[2] = z;
    d = fsqrt(x * x + z * z) + u->params->colR - 1100.0f;
    if (d > 0.0f) {
        v.x = x;
        v.y = 0.0f;
        v.z = z;
        VuVecNormalize(&v, &v);
        k = d * 0.1f + 0.5f;
        u->pos[0] -= v.x * k;
        u->pos[2] -= v.z * k;
    }
}
