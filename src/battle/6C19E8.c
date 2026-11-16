/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/n32.h"


#define ROUNDUP(x) ((x) == (f32)(s32)(x) ? (x) : ((x) > 0.0f ? (f32)((s32)(x) + 1) : (f32)~(s32)(-x)))

void AiEnemyChooseCommand(BtlUnit *u) {
    AiActionSet *act;
    BtlAction *cmd;
    BtlAction *c0;
    PtyMoveLevelData *lv;
    PtyUnit *st;
    s32 sum;
    s32 i;
    f32 r;

    if (btlPhase != 0x80) {
        return;
    }
    if (u->comboLeader >= 0) {
        u->action = 7;
        u->turn++;
        u->flag &= 0xFF000000;
        return;
    }
    if (u->params->status & 0x100) {
        u->ai = 0;
        act = (AiActionSet *)BTL_ACTION_SETS(u->def->data);
        u->action = 0;
        c0 = (BtlAction *)act->actions[0];
        u->skillId = c0->moveId;
        u->scopeRadius = ((PtyMoveLevelData *)*(s32 *)&c0->level)->scope; /* s32 load of level: register choice */
        u->scopeType = c0->scopeType;
        BtlUnitSelectTarget(u, 0x12, 1);
        if (u->target == u->id) {
            BtlUnitSelectTarget(u, 1, 1);
            u->scopeType = u->scopeType | 0x100;
        } else {
            u->scopeType = u->scopeType | 0x200;
        }
    } else {
        if (btlDebugCfg.aiNo >= 0) {
            u->ai = btlDebugCfg.aiNo;
            if (u->ai >= u->def->data->actionSetCount) {
                u->ai = u->def->data->actionSetCount - 1;
            }
            act = &((AiActionSet *)BTL_ACTION_SETS(u->def->data))[u->ai];
        } else {
            act = (AiActionSet *)AiEnemySelectActionSet(u);
        }
        if (btlDebugCfg.atkNo >= 0) {
            u->action = btlDebugCfg.atkNo;
        } else {
            sum = 0;
            r = (rand() >> 15) * (1.0f / 65536.0f);
            for (i = 0; i < 5; i++) {
                sum += act->prob[i];
            }
            for (i = 0; i < 4; i++) {
                if (act->prob[i] >= 100) {
                    break;
                }
                r -= (f32)act->prob[i] / (f32)sum;
                if (r <= 0.0f) {
                    break;
                }
            }
            u->action = i;
        }
        if ((BtlAction *)act->actions[u->action] == NULL) {
            u->action = 0;
        }
        if (u->def->validate != NULL) {
            u->def->validate(u);
            act = &((AiActionSet *)BTL_ACTION_SETS(u->def->data))[u->ai];
        }
        cmd = (BtlAction *)act->actions[u->action];
        lv = cmd->level;
        st = u->params;
        if ((lv->mpCost > 0 && ((f32)st->mp < ROUNDUP(lv->mpCost * st->mgcMpRate) || (st->status & 0x402)))
            || (lv->cost > 0 && ((f32)st->sp < ROUNDUP(lv->cost * st->spcSpRate) || (st->status & 0x200)))
            || (cmd->comboIds[0] > 0 && AiComboFindPartners(u, cmd, 0) == 0)) {
            u->action = 0;
            cmd = (BtlAction *)act->actions[0];
            lv = cmd->level;
        }
        u->skillId = cmd->moveId;
        u->scopeRadius = lv->scope;
        u->scopeType = cmd->scopeType;
        switch (cmd->targetSide) {
        case 3:
            u->target = u->id;
            break;
        case 1:
        case 5:
            BtlUnitSelectTarget(u, 2, cmd->targetCmd);
            break;
        default:
            BtlUnitSelectTarget(u, 1, cmd->targetCmd);
            break;
        }
        switch (cmd->scopeMode) {
        case 2:
            u->scopeType = u->scopeType | 0x100;
            break;
        case 4:
            u->scopeType |= 0x100;
        default:
            u->scopeType = u->scopeType | 0x200;
            break;
        }
    }
    u->turn++;
    u->flag &= 0xFF000000;
}
