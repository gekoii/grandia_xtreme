#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


void BtlUnitLungeInit(s32 *arg0) {
    BtlUnitLungeState *p;
    f32 v;

    v = BtlUnitGetMoveSpeed(arg0);
    p = &btlUnitLunge[*arg0];
    p->speed = v;
    if (v > 12.0f) {
        p->speed = 12.0f;
    }
    btlUnitLunge[*arg0].decel = p->speed * 0.03125f;
}
