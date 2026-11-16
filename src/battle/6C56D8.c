#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


s32 AiPickStatModSpell(BtlUnit *u, u32 mask) {
    PtyUnit *st;
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
    s32 kind;

    st = u->params;
    it = &ptyCharData[st->id - 0x1001];
    n = ptyCharDefTable[st->id - 0x1001].eggSlots;
    best = -1;
    res = -1;
    if (st->status & 0x402) {
        return -1;
    }
    mask &= ~btlUnits[u->target].powUp;
    if (mask == 0) {
        return -1;
    }
    if (mask >= 0x100) {
        kind = 1;
        mask >>= 8;
    } else {
        kind = 0;
    }
    for (i = 0; i < n; i++) {
        if ((u16)it->slots[i] < 0x18) {
            sk = &ptyPartyData->eggs[it->slots[i]];
            for (k = 0; k < 6; k++) {
                if (sk->spells[k] > 0 && sk->mp >= BtlUnitManaEggSpellCost(u, it->slots[i], k)) {
                    t = &ptyMoveTable[sk->spells[k]];
                    ef = &t->levels->effects[t->levels->effectCount - 1];
                    if (!(ef->statMask & mask)) {
                        continue;
                    }
                    if ((u32)((u8)ef->kind - 4) >= 2 && ef->kind != 7) {
                        if (kind == 0) {
                            continue;
                        }
                    } else if (kind == 1) {
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
