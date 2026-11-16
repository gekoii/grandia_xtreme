/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


s32 AiPickHealSpell(BtlUnit *u, s32 mode) {
    PtyUnit *st;
    PtyUnit *ts;
    PtyChar *it;
    PtyManaEgg *sk;
    PtyMove *t;
    PtyMoveEffect *ef;
    s32 n;
    s32 i;
    s32 k;
    s32 res;
    f32 best;
    f32 miss;
    f32 v;
    f32 p;

    st = u->params;
    ts = btlUnits[u->target].params;
    it = &ptyCharData[st->id - 0x1001];
    n = ptyCharDefTable[st->id - 0x1001].eggSlots;
    miss = ts->maxHp - ts->hp;
    best = 0.0f;
    res = -1;
    if (st->status & 0x402) {
        return -1;
    }
    for (i = 0; i < n; i++) {
        if ((u16)it->slots[i] < 0x18) {
            sk = &ptyPartyData->eggs[it->slots[i]];
            for (k = 0; k < 6; k++) {
                if (sk->spells[k] > 0 && sk->mp >= BtlUnitManaEggSpellCost(u, it->slots[i], k)) {
                    t = &ptyMoveTable[sk->spells[k]];
                    ef = &t->levels->effects[t->levels->effectCount - 1];
                    if (ef->kind != 1) {
                        continue;
                    }
                    if (t->target != 1) {
                        if (mode == 1) {
                            continue;
                        }
                    } else if (mode == 2) {
                        continue;
                    }
                    p = ef->power * 0.018749999f * st->magicRate[ef->element];
                    v = st->params[2] + ef->base + st->magicPow[ef->element];
                    v *= p;
                    v *= 1.3f;
                    if (miss <= best) {
                        if (miss <= v && v < best) {
                            best = v;
                            res = sk->spells[k];
                        }
                    } else if (best < v) {
                        res = sk->spells[k];
                        best = v;
                    }
                }
            }
        }
    }
    return res;
}
