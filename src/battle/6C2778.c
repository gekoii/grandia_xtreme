/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x006E1FE0-0x006E1FF8 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"
#include "battle/n32.h"


s32 AiPartyChooseCommand(BtlUnit *u) {
    f32 r;
    f32 q;
    BtlUnit *p;
    PtyUnit *st;
    s32 i;
    s32 j;

    if (u->params->status & 0x100) {
        r = (rand() >> 15) * (1.0f / 65536.0f) * 3.0f;
        if (r < 1.0f) {
            BtlUnitSelectTarget(u, 0x12, 1);
            if (u->target == u->id) {
                BtlUnitSelectTarget(u, 1, 1);
                AiCmdCritical(u);
            } else {
                AiCmdCritical(u);
                u->scopeType = (u->scopeType & ~0x200) | 0x100;
            }
        } else if (r < 2.0f) {
            AiCmdEvade(u);
        } else {
            AiCmdDefend(u);
        }
        return 1;
    }
    if (btlAutoTactics == 0) {
        return 0;
    }
    if (u->ai == 0) {
        return 0;
    }
    aiEnemyMaxRank = 0;
    aiBossHpRatio = 0.0f;
    aiStatusUsableMask = 0;
    p = btlEnemyUnits;
    for (i = 4; i < 16; i++, p++) {
        if (p->state >= 0x40) {
            st = p->params;
            if (aiEnemyMaxRank < st->size) {
                aiEnemyMaxRank = st->size;
            }
            if (st->size >= 5) {
                q = (f32)st->hp / (f32)st->maxHp;
                if (aiBossHpRatio < q) {
                    aiBossHpRatio = q;
                }
            }
            for (j = 0; j < 12; j++) {
                if (st->regSta[j] < 0.5f) {
                    aiStatusUsableMask |= 0x10 << j;
                }
            }
        }
    }
    aiStatusUsableMask = ~aiStatusUsableMask;
    switch (u->ai) {
    case 1:
        AiTacticAttack(u);
        break;
    case 2:
        AiTacticSingleTarget(u);
        break;
    case 3:
        AiTacticAreaAttack(u);
        break;
    case 4:
        AiTacticStatusAttack(u);
        break;
    case 5:
        AiTacticRecovery(u);
        break;
    case 6:
        AiTacticAssist(u);
        break;
    default:
        if (btlDebugCfg.flags & 0x20) {
            AiPartyRandomCommand(u);
        } else {
            AiTacticBalanced(u);
        }
        break;
    }
    return 1;
}
