/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/env.h"
#include "battle/unit.h"

void EnvDimStart(BtlUnit *u, s32 n, f32 target) {
    EnvDim *r = &envDim;
    s32 i;
    s32 j;

    r->target = target;
    if (n <= 0) {
        n = 1;
    }
    if (u != NULL) {
        r->mask = btlTargetMask;
        r->mask |= 1 << u->id;
        if (u->comboPartners[0] >= 0) {
            for (i = 0; i < 3; i++) {
                if (u->comboPartners[i] >= 0) {
                    r->mask |= 1 << u->comboPartners[i];
                }
            }
        }
        for (j = 0; j < 16; j++) {
            if ((r->mask >> j) & 1) {
                if (btlUnits[j].body >= 0) {
                    r->mask |= 1 << btlUnits[j].body;
                }
            }
        }
    }
    r->step = (r->target - r->cur) / n;
}
