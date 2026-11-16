/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "battle/n32.h"

void BtlTargetPickByDistance(BtlTargetSel *s, s32 i, s32 n, s32 max) {
    BtlUnit *u;
    BtlUnit *me;
    f32 dx;
    f32 dz;
    f32 d;
    f32 best;

    best = -1.0f;
    if (max == 0) {
        best = 9.99998e9f;
    }
    u = &btlUnits[i];
    for (; i < n; i++, u++) {
        if (s->flags[i] != 0) {
            me = s->self;
            dx = me->pos[0] - u->pos[0];
            dz = me->pos[2] - u->pos[2];
            d = dx * dx + dz * dz;
            if (max == 0 ? d < best : best < d) {
                s->idx = i;
                best = d;
            }
        }
    }
}
