/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n32.h"
#include "party.h"
#include "battle/unit.h"

#define CEIL(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))


s32 AiPickStatusSpecial(BtlUnit *u, s8 mode, s8 kind, s32 mask, u8 flags) {
    PtyUnit *st;
    PtyUnit *ts;
    PtyMoveLevel *e;
    PtyMove *t;
    PtyMoveLevelData *lv;
    PtyMoveEffect *ef;
    s32 i;
    s32 res;
    s32 best;
    s32 r;
    s32 hp;

    best = -1;
    res = -1;
    st = u->params;
    ts = btlUnits[u->target].params;
    if (st->status & 0x200) {
        return -1;
    }
    if (kind == 0) {
        mask &= aiStatusUsableMask;
        mask &= ~ts->status;
    } else {
        mask &= ts->status | 0xF;
    }
    if (mask == 0) {
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
        hp = u->params->sp;
        if (!(CEIL(lv->cost * u->params->spcSpRate) <= hp)) {
            continue;
        }
        ef = &lv->effects[lv->effectCount - 1];
        if (!(ef->flags & mask)) {
            continue;
        }
        if ((u32)((u8)ef->kind - 4) >= 2 && ef->kind != 7) {
            if (kind == 0) {
                continue;
            }
        } else if (kind != 0) {
            continue;
        }
        if (t->target != 1) {
            if (mode == 1) {
                continue;
            }
        } else if (mode == 2) {
            continue;
        }
        if (i >= 0x28) {
            if (!AiComboCanPerform(u, e->moveId, 0.25f)) {
                continue;
            }
        }
        r = rand() >> 15;
        if (best < r) {
            res = e->moveId;
            best = r;
        }
    }
    return res;
}
