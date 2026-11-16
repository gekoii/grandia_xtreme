/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n33.h"
#include "party.h"

void BtlHudGaugeUpdate(void) {
    BtlHudUnitGauge *g;
    PtyUnit *s;
    s32 i;
    s32 st;
    f32 tgt;

    g = btlHudUnitGauges;
    for (i = 0; i < 0x10; i++, g++) {
        st = g->hits;
        if (st < 0) {
            continue;
        }
        s = btlUnits[i].params;
        tgt = (f32)(s->maxHp - s->hp) / s->maxHp * 76.0f + 4.0f;
        if (tgt < g->barPos) {
            g->barPos -= 2.0f;
            if (g->barPos < tgt) {
                g->barPos = tgt;
            }
        } else if (g->barPos < tgt) {
            g->barPos += 2.0f;
            if (tgt < g->barPos) {
                g->barPos = tgt;
            }
        }
        if (st >= 2) {
            if (g->popIn < 1.0f) {
                g->popIn += 0.1f;
            }
            if (g->flash > 0.0f) {
                g->flash -= 0.03125f;
            }
        }
        if (g->shake > 0) {
            g->shake--;
        }
        if (g->timeout > 0) {
            g->timeout--;
            if (st == 0 && g->timeout <= 0) {
                g->hits = -1;
            }
        }
    }
}
