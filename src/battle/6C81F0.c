#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"


s32 BtlUnitApproachTarget(BtlUnit *arg0) {
    BtlUnit *u;
    f32 r;

    if (arg0->partsChr >= 0) {
        u = &btlUnits[arg0->partsChr];
    } else {
        u = arg0;
    }
    r = arg0->params->colR + btlUnits[u->target].params->colR;
    r += (u->id < 4) ? ptyMoveTable[u->skillId].reach : BTL_UNIT_ACT(u)->reach;
    return BtlUnitMoveToward(arg0, &btlUnits[u->target].pos, 3, BtlUnitGetMoveSpeed(arg0), r);
}
