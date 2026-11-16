#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n33.h"

/* BtlUnitMoveToward is (BtlUnit *, f32 *, s32, f32, f32); the call below declares it floats-first
   because EABI loads mixed int/float arguments in declaration order. */

void BtlUnitApproachTargetBy(BtlUnit *arg0, s32 mode, f32 r) {
    BtlUnit *u;
    BtlUnit *t;

    if (arg0->partsChr >= 0) {
        u = &btlUnits[arg0->partsChr];
    } else {
        u = arg0;
    }
    t = &btlUnits[u->target];
    r += arg0->params->colR + t->params->colR;
    r += (u->id < 4) ? ptyMoveTable[u->skillId].reach : BTL_UNIT_ACT(u)->reach;
    ((s32 (*)(BtlUnit *, VuVec3 *, f32, f32, s32))BtlUnitMoveToward)(arg0, &t->pos, BtlUnitGetMoveSpeed(u), r, mode);
}
