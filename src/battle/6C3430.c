/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/n32.h"


void AiTacticAssist(BtlUnit *u) {
    s32 k;

    BtlUnitSelectTarget(u, 2, 0x100D);
    if (AiTryHeal(u, 0.2f)) {
        return;
    }
    if (AiTryRevive(u)) {
        return;
    }
    BtlUnitSelectTarget(u, 2, 0x100D);
    if (AiTryHeal(u, 0.5f)) {
        return;
    }
    if (aiBuffOpenerReady != 0) {
        BtlUnitSelectTarget(u, 0x12, 0xE);
        if (AiTrySpecialMove(u, AiPickStatModSpecial(u, 0x100, 1), 0) || AiTrySpell(u, AiPickStatModSpell(u, 0x100))) {
            aiBuffOpenerReady = 0;
        }
        if (aiBuffOpenerReady == 0) {
            return;
        }
    }
    if (aiEnemyMaxRank >= 5 && (aiEnemyMaxRank >= 8 || btlEnemyAliveCount >= 4)) {
        if ((rand() >> 15) * (1.0f / 65536.0f) > 0.8f) {
            BtlUnitSelectTarget(u, 0x12, 1);
            if (AiTrySpecialMove(u, AiPickStatModSpecial(u, 0x900, 1), 0)) {
                return;
            }
            if (AiTrySpell(u, AiPickStatModSpell(u, 0x900))) {
                return;
            }
        }
        if ((rand() >> 15) * (1.0f / 65536.0f) > 0.8f) {
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
        k = aiEnemyMaxRank >= 8 ? 0xF : 9;
        u->target = u->id;
        if (AiTrySpecialMove(u, AiPickStatusSpecial(u, 0, 1, k, 1), 0)) {
            return;
        }
        if (AiTrySpell(u, AiPickStatusSpell(u, 0, 1, k))) {
            return;
        }
        BtlUnitSelectTarget(u, 0x12, 1);
        if (AiTrySpecialMove(u, AiPickStatusSpecial(u, 0, 1, k, 1), 0)) {
            return;
        }
        if (AiTrySpell(u, AiPickStatusSpell(u, 0, 1, k))) {
            return;
        }
    }
    AiCmdDefend(u);
}
