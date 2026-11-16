#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void BtlUnitUpdateAutoStatus(BtlUnit *arg0, s32 mask) {
    PtyUnit *s;
    s32 add;
    s32 i;

    add = 0;
    s = arg0->params;
    for (i = 0; i < 16; i++) {
        if ((mask >> i) & 1) {
            if ((arg0->params->status2 >> i) & 1) {
                mask &= ~(1 << i);
            }
        } else {
            if ((arg0->params->status2 >> i) & 1) {
                add |= 1 << i;
            }
            mask &= ~(1 << i);
        }
    }
    if (mask) {
        if (mask & 1) {
            arg0->transp = 0;
            s->status &= 0xFFFFFFFE;
        }
        if (mask & 2) {
            arg0->antiMagic = 0;
            s->status &= 0xFFFFFFFD;
        }
        if (mask & 8) {
            arg0->regene = 0;
            s->status &= 0xFFFFFFF7;
        }
        BtlHitQueueAddStatus(arg0, mask & 0xFFFFFFF4, 0, 0x200, 0);
    }
    if (add) {
        BtlHitQueueAddStatus(arg0, add, 0, 0, 0x5DC);
    }
}
