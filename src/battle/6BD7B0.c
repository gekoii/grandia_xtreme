/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "protos/battle.h"
#include "battle/n32.h"

void BtlTargetPickByCrowding(BtlTargetSel *s, s32 start, s32 n, s32 max) {
    BtlUnit *u;
    BtlUnit *v;
    s32 i;
    s32 j;
    f32 dx;
    f32 dz;
    f32 d;
    f32 sum;
    f32 best;

    best = -1.0f;
    if (max == 0) {
        best = 9.9998e9f;
    }
    i = start;
    u = &btlUnits[i];
    for (; i < n; i++, u++) {
        if (s->flags[i] != 0) {
            sum = (rand() >> 15) * (1.0f / 65536.0f);
            j = start;
            v = &btlUnits[j];
            for (; j < n; j++, v++) {
                if (s->flags[j] != 0) {
                    dx = u->pos[0] - v->pos[0];
                    dz = u->pos[2] - v->pos[2];
                    d = dx * dx + dz * dz;
                    if (d < 250000.0f) {
                        sum += d;
                    } else {
                        sum += 250000.0f;
                    }
                }
            }
            if (max == 0 ? sum < best : best < sum) {
                best = sum;
                s->idx = i;
            }
        }
    }
}
