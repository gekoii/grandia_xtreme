#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void BtlUnitRecalcParams(BtlUnit *arg0) {
    PtyUnit *s;
    s32 v;

    s = arg0->params;
    v = s->status2;
    PtyUnitCalcParams(s, 0, 0);
    BtlUnitUpdateAutoStatus(arg0, v);
}
