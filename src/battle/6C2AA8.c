#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"
#include "battle/n32.h"


void AiTacticSingleTarget(BtlUnit *u) {
    PtyUnit *ts;

    BtlUnitSelectTarget(u, 1, 0x100D);
    ts = btlUnits[u->target].params;
    if (aiEnemyMaxRank < 5) {
        if (ts->hp < ts->maxHp || AiTargetTakenByOther(u, 0, 4)) {
            goto tail; /* goto: shared tail (repeating it at each exit changes the layout) */
        }
        if (AiTargetCanBeCancelled(u) && AiTrySpecialMove(u, (u->params->id - 0x1001) * 6 + 0x65, 0)) {
            return;
        }
        if (u->params->sp == u->params->maxSp || (u->params->sp & 1)) {
            if (AiTrySpecialMove(u, AiPickDamageSpecial(u, 1, 1, 1), 0)) {
                return;
            }
        }
    } else {
        if (aiEnemyMaxRank < 8 && !(aiBossHpRatio >= 0.3f)) {
            if (ts->hp < ts->maxHp || AiTargetTakenByOther(u, 0, 4)) {
                goto tail;
            }
        }
        if (AiTargetCanBeCancelled(u) && AiTrySpecialMove(u, (u->params->id - 0x1001) * 6 + 0x65, 0)) {
            return;
        }
        if (u->params->sp == u->params->maxSp || (u->params->sp & 1)) {
            if (ts->size >= 5) {
                if (AiTrySpecialMove(u, AiPickDamageSpecial(u, 1, 2, 1), 0)) {
                    return;
                }
            }
            if (AiTrySpecialMove(u, AiPickDamageSpecial(u, 1, 1, 1), 0)) {
                return;
            }
        }
    }
    if (AiTrySpell(u, AiPickDamageSpell(u, 1))) {
        return;
    }
tail:
    AiCmdAttack(u);
}
