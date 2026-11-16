/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "party.h"
#include "battle/unit.h"


void AiPartyRandomCommand(BtlUnit *arg0) {
    PtyMove *t;
    f32 r;
    s16 n;
    s8 tries;
    s32 k;

    if (btlDebugCfg.mgcNo != 0) {
        r = 6.0f;
    } else {
        r = (rand() >> 15) * (1.0f / 65536.0f) * 6.0f;
    }
    if (r < 1.0f) {
        AiCmdEvade(arg0);
        return;
    }
    if (r < 2.0f) {
        AiCmdDefend(arg0);
        return;
    }
    if (r < 3.0f) {
        BtlUnitSelectTarget(arg0, 1, 3);
        AiCmdCombo(arg0);
        return;
    }
    if (r < 4.0f) {
        BtlUnitSelectTarget(arg0, 1, 3);
        AiCmdCritical(arg0);
        return;
    }
    if (btlDebugCfg.mgcNo == 0) {
        tries = 0;
        for (;;) {
            n = ((rand() >> 15) & 0xFF) + 1;
            if (n < 0x4A && n != 6) {
                tries++;
                if (tries >= 3) {
                    break;
                }
            }
            k = (arg0->params->id - 0x1001) * 6;
            if (n >= k + 0x65 && n < k + 0x6A) {
                break;
            }
            if (n >= 0xDD && AiComboCanPerform(arg0, n + 0x1000, 1.0f)) {
                break;
            }
        }
    } else {
        n = btlDebugCfg.mgcNo;
    }
    if (n < 0x65) {
        arg0->action = 3;
        arg0->skillLevel = 1;
        arg0->eggMagic[0] = 0;
        arg0->eggMagic[1] = 0;
        arg0->eggMagic[2] = 0;
        arg0->eggMagic[3] = 0;
        arg0->skillId = n;
        arg0->eggNo[0] = 0;
        arg0->eggNo[1] = 0;
        arg0->eggNo[2] = 0;
        arg0->eggNo[3] = 0;
        t = &ptyMoveTable[n];
        arg0->combo = btlDebugCfg.mgcCmb;
        arg0->scopeRadius = BtlSpellGetLevelData(arg0, 0, 0x10)->scope;
        BtlUnitSetTargetArea(arg0, t->target, t->unk1);
        BtlUnitFixTarget(arg0, t->kind, 0);
        BtlUnitSetState(arg0, 0xC0);
        return;
    }
    if (AiTrySpecialMove(arg0, n, 2)) {
        BtlUnitFixTarget(arg0, ptyMoveTable[arg0->skillId].kind, 0);
        return;
    }
    if ((rand() >> 15) <= 0x7FFF) {
        AiCmdDefend(arg0);
        return;
    }
    AiCmdEvade(arg0);
}
