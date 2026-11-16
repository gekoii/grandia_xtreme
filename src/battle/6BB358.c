#include "common.h"
#include "battle/unit.h"
#include "protos/battle.h"

void BtlSchedUpdate(void) {
    s32 r[16];
    s32 *q;
    s32 cur;
    s32 best;
    u32 i;
    s32 j;
    BtlUnit *u;
    BtlUnit *v;
    s32 k;

    if (btlActDoneMask != 0) {
        BtlSchedActFinish();
        btlActDoneMask = 0;
    }
    if (btlActingUnit == 0xFF) {
        BtlUnitsCheckDefeated();
        if (btlKoRequestMask != 0) {
            BtlSchedKoBegin();
        }
    } else if (btlActingUnit == 0xFE) {
        if (btlKoPendingMask == 0) {
            BtlSchedActFinish();
        }
    }
    cur = btlActingUnit;
    if (cur != 0xFF) {
        return;
    }
    if (BtlVictoryCheck() != 0) {
        btlActRequestMask = 0;
    }
    if (btlActingUnit != cur) {
        return;
    }
    v = btlUnits;
    for (j = 0; j < 16; j++, v++) {
        if (v->state < 0x80) {
            btlActRequestMask &= ~(1 << j);
        }
    }
    if (btlActRequestMask == 0) {
        return;
    }
    u = NULL;
    if (btlActRequestMask & 0x10000) {
        btlActingUnit = 0xFC;
        btlActRequestMask &= ~0x10000;
    } else {
        q = &r[15];
        for (i = 0x8000; i != 0; q--) {
            if (btlActRequestMask & i) {
                i >>= 1;
                *q = rand() >> 15;
            } else {
                *q = -1;
                i >>= 1;
            }
        }
        best = -1;
        q = r;
        for (; i < 16; i++, q++) {
            if (best < *q) {
                best = *q;
                u = &btlUnits[i];
            }
        }
        btlActingUnit = u->id;
        btlActRequestMask &= ~(1 << u->id);
        u->flags |= 8;
    }
    u = btlUnits;
    for (k = 0; k < 16; k++, u++) {
        if ((u32)(u->state - 1) < 4) {
            u->flags |= 8;
        }
    }
}
