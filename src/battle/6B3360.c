#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n31.h"


void MgcFallbackMagicHit(MgcWork *t) {
    BtlUnit *id = t->caster;

    switch (t->phase) {
    case 0x114:
        BtlUnitHitAreaTargets(id, 0);
        t->count = 0;
        t->phase++;
    case 0x115:
        if (t->count >= 50) {
            MgcSetNextProc(MgcFallbackMagicEnd);
            t->phase = 0x11E;
        }
        break;
    }
}
