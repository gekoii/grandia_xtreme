#include "common.h"
#include "battle/misc.h"
#include "battle/unit.h"

extern s32 fxDelayedCastStep;

void MgcSetNextProc(MgcProc arg0) {
    BtlUnit *c;
    BtlUnit *e;

    mgcWork.next = arg0;
    c = mgcWork.caster;
    if (c != NULL) {
        if (c->body >= 0) {
            e = &btlUnits[c->body];
            e->phase++;
            c = &btlUnits[c->body];
            c->subPhase = 0;
        } else {
            c->subPhase = 0;
            c->phase++;
        }
    } else {
        fxDelayedCastStep++;
    }
}
