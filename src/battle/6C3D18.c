#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void AiCmdAttack(BtlUnit *arg0) {
    PtyUnit *st;
    s32 r;

    r = AiTargetCanBeCancelled(arg0);
    st = arg0->params;
    if (r != 0) {
        if (st->cmdDisable & 0x20) {
            AiCmdCombo(arg0);
            return;
        }
        AiCmdCritical(arg0);
        return;
    }
    if (st->cmdDisable & 1) {
        AiCmdCritical(arg0);
        return;
    }
    AiCmdCombo(arg0);
}
