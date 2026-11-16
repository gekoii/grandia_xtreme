/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"
#include "party.h"


s32 AiPickDamageSpell(BtlUnit *u, s32 mode) {
    PtyUnit *st;
    s32 n;
    PtyUnit *ts;
    PtyChar *it;
    PtyManaEgg *sk;
    PtyMove *t;
    PtyMoveLevelData *lv;
    PtyMoveEffect *e;
    s32 i;
    s32 k;
    s32 res;
    f32 best;
    f32 v;

    st = u->params;
    it = &ptyCharData[st->id - 0x1001];
    n = ptyCharDefTable[st->id - 0x1001].eggSlots;
    ts = btlUnits[u->target].params;
    best = 0.0f;
    res = -1;
    if (st->status & 0x402) {
        return -1;
    }
    for (i = 0; i < n; i++) {
        if ((u16)it->slots[i] < 0x18) {
            sk = &ptyPartyData->eggs[it->slots[i]];
            for (k = 0; k < 6; k++) {
                if (sk->spells[k] > 0 && sk->mp >= ((s32 (*)(BtlUnit *, s16, s32))BtlUnitManaEggSpellCost)(u, (u16)it->slots[i], k)) { /* cast: the slot goes through an s16 parameter */
                    t = &ptyMoveTable[sk->spells[k]];
                    lv = t->levels;
                    e = &lv->effects[lv->effectCount - 1];
                    if ((u32)((u8)e->kind - 4) < 2) {
                        if (!(ts->regMgcAttr[e->element] < 0.8f)) {
                            if (t->target != 1) {
                                if (mode == 1) {
                                    continue;
                                }
                            } else if (mode == 2) {
                                continue;
                            }
                            v = lv->mpCost * ts->regMgcAttr[e->element];
                            if (best < v) {
                                res = sk->spells[k];
                                best = v;
                            }
                        }
                    }
                }
            }
        }
    }
    return res;
}
