#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "party.h"


s32 BtlHitDrainHp(BtlHitSource *r, BtlUnit *u, s32 n) {
    VuVec4 v;
    s32 i;

    if (r->unit < 4) {
        for (i = 0; i < 2; i++) {
            if (u->params->race[i] == 6) {
                break;
            }
        }
    } else {
        i = 2;
    }
    if (i < 2) {
        if (r->params->hp < n) {
            n = r->params->hp;
        }
        BtlUnitApplyDamage(&btlUnits[r->unit], n, 0, 0);
        BtlChrGetPartPos(&v, &btlUnits[r->unit], 1);
        FxDrainOrbStart(u, &v, n);
        BtlHudDamageNumSpawn(n, &v, 0);
        return 0;
    }
    if (u->params->hp < n) {
        n = u->params->hp;
    }
    BtlChrGetPartPos(&v, u, 1);
    if (btlActingUnit == r->unit && (btlUnits + r->unit)->skillId == 0x42) {
        v.y += 500.0f;
    }
    FxDrainOrbStart(&btlUnits[r->unit], &v, n);
    return 1;
}
