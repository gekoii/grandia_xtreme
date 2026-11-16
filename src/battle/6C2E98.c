/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n32.h"
#include "battle/unit.h"


void AiTacticStatusAttack(BtlUnit *u) {
    if (aiEnemyMaxRank < 5) {
        if (aiStatusOpenerReady == 0) {
            goto tail; /* goto: shared tail (repeating it at each exit changes the layout) */
        }
        BtlUnitSelectTarget(u, 1, 0x1009);
        if (AiTrySpecialMove(u, AiPickStatusSpecial(u, 2, 0, 0x1F0, 1), 0) || AiTrySpell(u, AiPickStatusSpell(u, 2, 0, 0x1F0))) {
            aiStatusOpenerReady = 0;
        }
        if (aiStatusOpenerReady == 0) {
            return;
        }
        goto tail;
    } else if (aiEnemyMaxRank < 8) {
        BtlUnitSelectTarget(u, 1, 0x1009);
        if (aiStatusOpenerReady != 0) {
            if (AiTrySpecialMove(u, AiPickStatusSpecial(u, 2, 0, 0x7F0, 1), 0) || AiTrySpell(u, AiPickStatusSpell(u, 2, 0, 0x7F0))) {
                aiStatusOpenerReady = 0;
            }
            if (aiStatusOpenerReady == 0) {
                return;
            }
        }
        if (!(aiBossHpRatio >= 0.3f)) {
            goto tail;
        }
        if ((rand() >> 15) * (1.0f / 65536.0f) < 0.5f) {
            if (AiTrySpecialMove(u, AiPickStatModSpecial(u, 0x1F, 3), 0)) {
                return;
            }
            if (AiTrySpell(u, AiPickStatModSpell(u, 0x1F))) {
                return;
            }
        }
        if (AiTrySpecialMove(u, AiPickStatusSpecial(u, 0, 0, 0x7F0, 3), 0)) {
            return;
        }
        if (AiTrySpell(u, AiPickStatusSpell(u, 0, 0, 0x7F0))) {
            return;
        }
    } else {
        aiStatusOpenerReady = 0;
        BtlUnitSelectTarget(u, 1, 0x1009);
        if ((rand() >> 15) * (1.0f / 65536.0f) < 0.5f) {
            if (AiTrySpecialMove(u, AiPickStatModSpecial(u, 0x1F, 3), 0)) {
                return;
            }
            if (AiTrySpell(u, AiPickStatModSpell(u, 0x1F))) {
                return;
            }
        }
        if (AiTrySpecialMove(u, AiPickStatusSpecial(u, 0, 0, 0xFFF0, 3), 0)) {
            return;
        }
        if (AiTrySpell(u, AiPickStatusSpell(u, 0, 0, 0xFFF0))) {
            return;
        }
    }
tail:
    BtlUnitSelectTarget(u, 1, 0x107F);
    AiCmdAttack(u);
}
