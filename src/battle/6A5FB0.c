/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"


void BtlStartApplyStatus(void) {
    BtlUnit *u = btlUnits;
    PtyUnit *info;
    s32 i;
    s32 fl;

    for (i = 15; i >= 0; i--, u++) {
        fl = 0;
        info = u->params;
        if (u->state < 5) {
            continue;
        }
        if (info->startTransp > 0.0f) {
            if ((rand() >> 15) * (1.0f / 65536.0f) <= info->startTransp) {
                fl = 1;
            }
        }
        if (info->startSleep > 0.0f) {
            if ((rand() >> 15) * (1.0f / 65536.0f) <= info->startSleep) {
                fl |= 0x20;
            }
        }
        if (info->startPara > 0.0f) {
            if ((rand() >> 15) * (1.0f / 65536.0f) <= info->startPara) {
                fl |= 0x40;
            }
        }
        BtlHitQueueAddStatus(u, fl | info->status2, 0, 0, 0x5DC);
        if (info->bitCsf & 0x4000000) {
            BtlHitQueueAddStatus(u, 0, 0x1F00, 0, 0x5DC);
        }
    }
}
