#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void AiCmdCombo(BtlUnit *u) {
    PtyUnit *st;

    if (u->target == u->id || (st = u->params, st->cmdDisable & 1)) {
        AiCmdEvade(u);
        return;
    }
    u->action = 0; //P
    u->skillId = u->def->id - 0xF4C; //P
    u->skillLevel = 1; //P
    u->scopeType = 0x201; //P
    if (!(st->status & 0x200)) {
        BtlCmdTryLearnMove(u);
    }
    BtlUnitSetState(u, 0xC0);
}
