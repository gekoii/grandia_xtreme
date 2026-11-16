#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "protos/battle.h"

void FxTrailSetUnitColor(s32 a, BtlUnit *u, s8 flag) {
    BtlUnit *p;
    s32 k;
    PtyMoveEffect *inf;

    if (u->partsChr >= 0) {
        p = &btlUnits[u->partsChr];
    } else {
        p = u;
    }
    if (p->id < 4) {
        if (p->action >= 2 && p->state >= 0xC0) {
            k = MgcGetHitData()->element;
        } else {
            k = p->params->eqpAttr;
        }
    } else {
        if (p->skillId == 0) {
            inf = BTL_UNIT_ACT(p)->level->effects;
        } else {
            inf = MgcGetHitData();
        }
        k = inf->element;
    }
    FxTrailSetColorKind(a, k, flag);
}
