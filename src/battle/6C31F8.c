#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/n32.h"


void AiTacticRecovery(BtlUnit *u) {
    u->target = u->id;
    if (!AiTargetTakenByOther(u, 4, 0x10)) {
        BtlUnitSelectTarget(u, 2, 0x100D);
        if (AiTryHeal(u, 0.4f)) {
            return;
        }
        if (AiTryRevive(u)) {
            return;
        }
        BtlUnitSelectTarget(u, 2, 0x102F);
        if (AiTrySpecialMove(u, AiPickStatusSpecial(u, 0, 1, 0x7FF0, 1), 0)) {
            return;
        }
        if (AiTrySpell(u, AiPickStatusSpell(u, 0, 1, 0x7FF0))) {
            return;
        }
        BtlUnitSelectTarget(u, 2, 0x100D);
        if (AiTryHeal(u, 0.7f)) {
            return;
        }
        if (aiEnemyMaxRank >= 5 && (aiEnemyMaxRank >= 8 || btlEnemyAliveCount >= 4)) {
            u->target = u->id;
            if (AiTrySpecialMove(u, AiPickStatModSpecial(u, 0x600, 1), 0)) {
                return;
            }
            if (AiTrySpell(u, AiPickStatModSpell(u, 0x600))) {
                return;
            }
            BtlUnitSelectTarget(u, 0x12, 1);
            if (AiTrySpecialMove(u, AiPickStatModSpecial(u, 0x600, 1), 0)) {
                return;
            }
            if (AiTrySpell(u, AiPickStatModSpell(u, 0x600))) {
                return;
            }
        }
        if (aiEnemyMaxRank < 8) {
            BtlUnitSelectTarget(u, 1, 0x107F);
            AiCmdAttack(u);
            return;
        }
    }
    AiCmdDefend(u);
}
