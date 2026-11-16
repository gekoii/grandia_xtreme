/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

f32 BtlCamFarthestUnitDist(s32 *out, f32 x, f32 z) {
    BtlUnit *c;
    f32 best;
    f32 dx, dz, d;
    s32 i, idx;

    best = 0.0f;
    c = btlUnits;
    idx = 0;
    for (i = 0; i < 16; i++, c++) {
        if (c->state >= 0x40) {
            dx = c->pos[0] - x;
            dz = c->pos[2] - z;
            d = dx * dx + dz * dz;
            if (best < d) {
                best = d;
                idx = i;
            }
        }
    }
    if (out != NULL) {
        *out = idx;
    }
    return fsqrt(best);
}
