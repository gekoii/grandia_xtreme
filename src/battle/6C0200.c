#include "common.h"
#include "battle/unit.h"
#include "battle/n32.h"
#include "protos/battle.h"

void BtlUnitHitAreaTargets(BtlUnit *arg0, s32 arg1) {
    BtlUnit *u;

    switch ((u8)arg0->scopeType) {
    case 5:
        BtlUnitHitAreaTargetsFrom(arg0, arg1, 0.0f, 0.0f);
        return;
    case 2:
    case 6:
        BtlUnitHitAreaTargetsFrom(arg0, arg1, btlAreaCtrX, btlAreaCtrZ);
        return;
    }
    if (arg0->body >= 0) {
        u = &btlUnits[arg0->body];
        BtlUnitHitAreaTargetsFrom(arg0, arg1, u->pos[0], u->pos[2]);
        return;
    }
    BtlUnitHitAreaTargetsFrom(arg0, arg1, arg0->pos[0], arg0->pos[2]);
}
