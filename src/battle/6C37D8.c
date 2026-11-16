/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n32.h"
#include "party.h"
#include "battle/unit.h"


void AiTacticBalanced(BtlUnit *u) {
    s32 r;
    u32 m;
    s32 n;

    BtlUnitSelectTarget(u, 2, 0x100D);
    if (AiTryHeal(u, 0.2f)) {
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
    if (AiTryHeal(u, 0.5f)) {
        return;
    }
    if (aiAreaOpenerReady != 0) {
        BtlUnitSelectTarget(u, 1, 0x1009);
        if (AiTrySpecialMove(u, AiPickStatusSpecial(u, 2, 0, 0x1F0, 1), 0) || AiTrySpell(u, AiPickStatusSpell(u, 2, 0, 0x1F0)) || AiTrySpecialMove(u, AiPickDamageSpecial(u, 2, 1, 0), 0) || AiTrySpell(u, AiPickDamageSpell(u, 2))) {
            aiAreaOpenerReady = 0;
        }
        if (aiAreaOpenerReady == 0) {
            return;
        }
    }
    if (aiEnemyMaxRank >= 5 && (aiEnemyMaxRank >= 8 || aiBossHpRatio >= 0.3f)) {
        BtlUnitSelectTarget(u, 1, 0x1009);
        if (AiTargetCanBeCancelled(u) && AiTrySpecialMove(u, (u->params->id - 0x1001) * 6 + 0x65, 0)) {
            return;
        }
        r = 0;
        if (u->params->sp == u->params->maxSp || (u->params->sp & 1)) {
            r = AiTrySpecialMove(u, AiPickDamageSpecial(u, 2, 1, 1), 0) != 0;
        }
        if (r || AiTrySpell(u, AiPickDamageSpell(u, 2))) {
            m = ((u32 (*)(BtlUnit *))BtlUnitCollectAreaTargetsAtTarget)(u);
            n = 0;
            while (m) {
                n += m & 1;
                m >>= 1;
            }
            if (n >= 3) {
                return;
            }
        }
        BtlUnitSelectTarget(u, 1, 0x107E);
        if (AiTargetCanBeCancelled(u) && AiTrySpecialMove(u, (u->params->id - 0x1001) * 6 + 0x65, 0)) {
            return;
        }
        if (u->params->sp == u->params->maxSp || (u->params->sp & 1)) {
            if (AiTrySpecialMove(u, AiPickDamageSpecial(u, 1, 1, 1), 0)) {
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
