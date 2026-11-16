/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"
#include "battle/n32.h"

#define CEIL(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))


void BtlEnemyActionStart(BtlUnit *u) {
    s32 k;
    PtyUnit *st;
    BtlAction *sk;
    PtyMoveLevelData *c;

    k = u->action;
    st = u->params;
    sk = BTL_ACTION_SETS(u->def->data)[u->ai].actions[k];
    c = sk->level;
    if (k == 8 || k < 0 || k >= 5 || sk == NULL) {
        AiCmdEvade(u);
        u->action = 0x7F;
        return;
    }
    if ((c->mpCost > 0 && (st->mp < CEIL(c->mpCost * st->mgcMpRate) || (st->status & 0x402)))
        || (c->cost > 0 && (st->sp < CEIL(c->cost * st->spcSpRate) || (st->status & 0x200)))
        || (sk->comboIds[0] > 0 && AiComboFindPartners(u, sk, 1) == 0)) {
        u->action = 0;
        sk = BTL_ACTION_SETS(u->def->data)[u->ai].actions[0];
        u->skillId = sk->moveId;
        c = sk->level;
        u->scopeRadius = c->scope;
        u->scopeType = sk->scopeType | 0x100;
        BtlUnitSelectTarget(u, 1, 1);
    }
    if (!(st->status & 0x100)) {
        if (sk->targetSide == 2 && u->target >= 4) {
            BtlUnitSelectTarget(u, 1, sk->targetCmd);
            u->scopeType = sk->scopeType | 0x100;
        }
        if (BtlCmdHasTarget(u, sk->targetSide) == 0) {
            AiCmdEvade(u);
            u->action = 0x7F;
            return;
        }
    }
    if (sk->comboIds[0] > 0 && AiComboJoin(u, sk) == 0) {
        AiCmdEvade(u);
        u->action = 0x7F;
        return;
    }
    BtlUnitSetState(u, 0xC0);
}
