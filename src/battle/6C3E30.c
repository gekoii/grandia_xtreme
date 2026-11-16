#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void AiCmdDefend(BtlUnit *arg0) {
    if (arg0->params->cmdDisable & 2) {
        AiCmdEvade(arg0);
        return;
    }
    BtlUnitSelectTarget(arg0, 1, 0x1003);
    BtlUnitSetState(arg0, 0x84);
}
