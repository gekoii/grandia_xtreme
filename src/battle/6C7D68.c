/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "party.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlUnitSeparationPush(BtlUnit *u, f32 *ox, f32 *oz) {
    BtlUnit *t;
    s32 i;
    PtyUnit *s;
    f32 dx, dz, d;

    t = btlUnits;
    *ox = *oz = 0.0f;
    i = 15;
    do {
        if (t->state >= 4 && t->alpha > 0.0f) {
            s = t->params;
            if (!(s->bitCsf & 0x200) && u->id != t->id) {
                dx = u->pos[0] - t->pos[0];
                dz = u->pos[2] - t->pos[2];
                d = fsqrt(dx * dx + dz * dz) - 10.0f;
                if (d < s->colR + u->params->colR) {
                    if (d <= 0.0f) {
                        *oz += 4.0f;
                    } else {
                        *ox += dx / d * 4.0f;
                        *oz += (u->pos[2] - t->pos[2]) / d * 4.0f;
                    }
                }
            }
        }
        i--;
        t++;
    } while (i >= 0);
}
