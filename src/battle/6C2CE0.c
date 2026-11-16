#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"


void AiTacticAreaAttack(BtlUnit *u) {
    if (btlEnemyAliveCount >= 4) {
        BtlUnitSelectTarget(u, 1, 0x1009);
        if (AiTargetCanBeCancelled(u) && AiTrySpecialMove(u, (u->params->id - 0x1001) * 6 + 0x65, 0)) {
            return;
        }
        if (u->params->sp == u->params->maxSp || (u->params->sp & 1)) {
            if (AiTrySpecialMove(u, AiPickDamageSpecial(u, 2, 3, 1), 0)) {
                return;
            }
        }
        if (AiTrySpell(u, AiPickDamageSpell(u, 2))) {
            return;
        }
    } else {
        BtlUnitSelectTarget(u, 1, 0x107E);
        if (AiTargetCanBeCancelled(u) && AiTrySpecialMove(u, (u->params->id - 0x1001) * 6 + 0x65, 0)) {
            return;
        }
        if (u->params->sp == u->params->maxSp || (u->params->sp & 1)) {
            if (AiTrySpecialMove(u, AiPickDamageSpecial(u, 1, 3, 1), 0)) {
                return;
            }
        }
        if (AiTrySpell(u, AiPickDamageSpell(u, 1))) {
            return;
        }
    }
    BtlUnitSelectTarget(u, 1, 0x107F);
    AiCmdAttack(u);
}
