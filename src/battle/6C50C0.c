#include "common.h"
#include "protos/battle.h"
#include "battle/n32.h"
#include "party.h"
#include "battle/unit.h"


s32 AiPickStatusSpell(BtlUnit *u, s8 mode, s8 kind, s32 mask) {
    PtyUnit *st;
    PtyUnit *ts;
    PtyChar *it;
    PtyManaEgg *sk;
    PtyMove *t;
    PtyMoveEffect *ef;
    s32 n;
    s32 i;
    s32 k;
    s32 best;
    s32 res;
    s32 r;

    st = u->params;
    it = &ptyCharData[st->id - 0x1001];
    n = ptyCharDefTable[st->id - 0x1001].eggSlots;
    ts = btlUnits[u->target].params;
    best = -1;
    res = -1;
    if (st->status & 0x402) {
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
    for (i = 0; i < n; i++) {
        if ((u16)it->slots[i] < 0x18) {
            sk = &ptyPartyData->eggs[it->slots[i]];
            for (k = 0; k < 6; k++) {
                if (sk->spells[k] > 0 && sk->mp >= BtlUnitManaEggSpellCost(u, it->slots[i], k)) {
                    t = &ptyMoveTable[sk->spells[k]];
                    ef = &t->levels->effects[t->levels->effectCount - 1];
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
                    r = rand() >> 15;
                    if (best < r) {
                        best = r;
                        res = sk->spells[k];
                    }
                }
            }
        }
    }
    return res;
}
