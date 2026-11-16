/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

#define CEIL(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))


s32 AiTrySpecialMove(BtlUnit *u, s16 id, s8 mode) {
    s32 lv;
    PtyUnit *st;
    s16 a;
    s32 base;
    PtyMove *t;
    s32 hp;

    if (id < 0x65) {
        return 0;
    }
    lv = ptyPartyData->moveLevels[BtlSpecialMoveRecordIndex(id)].level;
    if (mode != 0 && lv == 0) {
        lv = 1;
    }
    if (lv <= 0 || ((st = u->params)->status & 0x200)) {
        return 0;
    }
    if (id >= 0xDD) {
        a = id;
        if (mode == 2) {
            a = id + 0x1000;
        }
        if (!AiComboCanPerform(u, a, 1.0f)) {
            return 0;
        }
        if (!BtlCmdHasTarget(u, ptyMoveTable[id].kind)) {
            return 0;
        }
        u->skillId = id;
        u->skillLevel = lv;
        u->action = 4;
        u->combo = 1;
        BtlCmdComboLinkPartners(u);
    } else {
        base = (st->id - 0x1001) * 6;
        if (id < base + 0x65) {
            return 0;
        }
        if (id >= base + 0x6A) {
            return 0;
        }
        if (mode == 0) {
            hp = st->sp;
            if (hp < CEIL(ptyMoveTable[id].levels[lv - 1].cost * st->spcSpRate)) {
                return 0;
            }
        }
        if (!BtlCmdHasTarget(u, ptyMoveTable[id].kind)) {
            return 0;
        }
        u->action = 4;
        u->skillLevel = lv;
        u->skillId = id;
        u->combo = 1;
        if (mode != 1) {
            BtlCmdTryLearnCombo(u);
            if (u->skillId >= 0xDD) {
                BtlCmdComboLinkPartners(u);
            }
        }
    }
    t = &ptyMoveTable[u->skillId];
    u->scopeRadius = t->levels[u->skillLevel - 1].scope;
    BtlUnitSetTargetArea(u, t->target, t->unk1);
    BtlUnitSetState(u, 0xC0);
    return 1;
}
