#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


void BtlUnitStateCommand(BtlUnit *arg0) {
    BtlUnitTurnToTarget(arg0);
    if (arg0->subPhase == 0) {
        BtlCmdMenuOpen(arg0);
        BtlChrSetMotion(arg0, 5);
        arg0->subPhase++;
    }
}
