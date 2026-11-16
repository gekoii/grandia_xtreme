/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

#define CEIL(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))


s32 AiPickDamageSpecial(BtlUnit *u, s8 mode, s8 flags, u8 ff) {
    PtyUnit *st;
    PtyUnit *ts;
    PtyMoveLevel *e;
    PtyMove *t;
    PtyMoveLevelData *lv;
    PtyMoveEffect *ef;
    s32 i;
    s32 res;
    s32 cost;
    f32 best;
    f32 v;
    s32 hp;

    best = 0.0f;
    st = u->params;
    cost = 0;
    ts = btlUnits[u->target].params;
    res = -1;
    if (st->status & 0x200) {
        return -1;
    }
    i = (st->id - 0x1001) * 5;
    e = &ptyPartyData->moveLevels[i];
    for (; i < 0x5A; i++, e++) {
        if (i >= 0x28) {
            if (!(flags & 2)) {
                break;
            }
        } else if (!(flags & 1) || i >= (u->params->id - 0x1001) * 5 + 5) {
            i = 0x27;
            e = &ptyPartyData->moveLevels[0x27];
            continue;
        }
        t = &ptyMoveTable[e->moveId];
        lv = &t->levels[e->level - 1];
        if (e->level == 0) {
            continue;
        }
        ef = &lv->effects[lv->effectCount - 1];
        if ((u32)((u8)ef->kind - 4) >= 2) {
            continue;
        }
        if (ts->regMgcAttr[ef->element] < 0.8f) {
            continue;
        }
        if (t->target != 1) {
            if (mode == 1) {
                continue;
            }
        } else if (mode == 2) {
            continue;
        }
        if (ff == 0) {
            hp = u->params->sp;
            if (hp < CEIL(lv->cost * u->params->spcSpRate)) {
                continue;
            }
        }
        if (i >= 0x28) {
            if (!AiComboCanPerform(u, e->moveId, 0.25f)) {
                continue;
            }
        }
        v = lv->cost * ts->regMgcAttr[ef->element] * BtlMoveMemberCount(e->moveId);
        if (best < v) {
            best = v;
            cost = lv->cost;
            res = e->moveId;
        }
    }
    if (ff == 0) {
        return res;
    }
    hp = u->params->sp;
    if (hp < CEIL(cost * u->params->spcSpRate)) {
        return -1;
    }
    return res;
}
