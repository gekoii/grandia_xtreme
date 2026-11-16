/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "battle/n32.h"

#define HEIGHT(u) ((u)->ipPos + ((u)->ipMode != 2 ? 1.0f : 0.0f))
#define ABS(x) ((x) < 0.0f ? -(x) : (x))

void BtlTargetPickByIp(BtlTargetSel *s, s32 i, s32 n, f32 h) {
    BtlUnit *u;
    f32 d;
    f32 best;

    best = 99999.9f;
    u = &btlUnits[i];
    for (; i < n; i++, u++) {
        if (s->flags[i] != 0) {
            d = ABS(HEIGHT(u) - h);
            if (d < best) {
                s->idx = i;
                best = d;
            }
        }
    }
}
