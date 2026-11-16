#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"


void BtlHitQueueProcess(void) {
    BtlHit *h;
    BtlUnit *u;
    s32 i;
    s32 r;

    h = btlHitQueue;
    for (i = 0; i < btlHitQueueCount; i++, h++) {
        r = BtlHitResolve(h);
        if (h->callback != NULL) {
            h->callback(h->callbackArg, r, h->target);
        }
    }
    btlHitQueueCount = 0;
    u = btlUnits;
    for (i = 0; i < 16; i++, u++) {
        if (btlKnockbackAccum[i] > 0) {
            BtlHitQueueAddDamage(u, btlKnockbackAccum[i], 0.0f, u->rot[1], 0x100, 0);
        } else if (btlKnockbackAccum[i] < 0) {
            BtlHitQueueAddDamage(u, -btlKnockbackAccum[i], 0.0f, u->rot[1], 0x6800, 0);
        }
        btlKnockbackAccum[i] = 0;
    }
    h = btlHitQueue;
    i = 0;
    if (i < btlHitQueueCount) {
        do {
            i++;
            BtlHitResolve(h);
            h++;
        } while (i < btlHitQueueCount);
    }
    btlHitQueueCount = 0;
}
