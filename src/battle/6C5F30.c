/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

#define ROUNDUP(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))

s32 AiPickHealSpecial(BtlUnit *u, s32 side) {
    PtyUnit *st;
    PtyPartyData *b;
    PtyUnit *ts;
    PtyMove *e;
    PtyMoveLevelData *lv;
    PtyMoveEffect *a;
    s32 i;
    s32 id;
    s32 best;
    s32 n;
    s32 k;
    f32 need;
    f32 bestv;
    f32 v;

    best = -1;
    bestv = 0.0f;
    st = u->params;
    ts = btlUnits[u->target].params;
    need = ts->maxHp - ts->hp;
    if (st->status & 0x200) {
        return -1;
    }
    for (i = 0; i < 5; i++) {
        b = ptyPartyData;
        k = i + 0x65;
        id = (u->params->id - 0x1001) * 6 + k;
        n = b->moveLevels[BtlSpecialMoveRecordIndex(id)].level;
        e = &ptyMoveTable[id];
        lv = &e->levels[n - 1];
        if (n <= 0) {
            continue;
        }
        if (!(ROUNDUP(lv->cost * u->params->spcSpRate) <= (f32)u->params->sp)) {
            continue;
        }
        a = &lv->effects[lv->effectCount - 1];
        if (a->kind != 1) {
            continue;
        }
        if (e->target != a->kind) {
            if (side == a->kind) {
                continue;
            }
        } else if (side == 2) {
            continue;
        }
        v = (f32)(st->params[2] + a->base + st->magicPow[a->element]);
        v *= (f32)a->power * 0.018749999f * st->magicRate[a->element];
        v *= 1.3f;
        if (need <= bestv) {
            if (!(need <= v)) continue;
            if (!(v < bestv)) continue;
        } else if (!(bestv < v)) continue;
        bestv = v;
        best = id;
    }
    return best;
}
