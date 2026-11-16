#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


s32 AiTrySpell(BtlUnit *u, s32 id) {
    PtyChar *it;
    s32 n;
    PtyUnit *st;
    s32 k;
    PtyMove *tb;
    PtyManaEgg *sk;
    s32 i;

    st = u->params;
    it = &ptyCharData[st->id - 0x1001];
    n = ptyCharDefTable[st->id - 0x1001].eggSlots;
    if ((st->status & 0x402) || id <= 0) {
        return 0;
    }
    if (!BtlCmdHasTarget(u, ptyMoveTable[id].kind)) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        if ((u16)it->slots[i] < 0x18) {
            sk = &ptyPartyData->eggs[it->slots[i]];
            for (k = 0; k < 6; k++) {
                if (id == sk->spells[k]) {
                    if (sk->mp >= BtlUnitManaEggSpellCost(u, it->slots[i], k)) {
                        u->action = 3;
                        u->eggNo[0] = it->slots[i];
                        u->eggMagic[0] = k;
                        u->skillId = id;
                        u->skillLevel = 1;
                        tb = &ptyMoveTable[id];
                        u->combo = 1;
                        u->scopeRadius = ((PtyMoveLevelData *(*)(BtlUnit *, s32, s32, s32))BtlSpellGetLevelData)(u, 0, 0x10, 1)->scope; /* cast: a fourth argument (see the definition) */
                        BtlUnitSetTargetArea(u, tb->target, tb->unk1);
                        BtlUnitSetState(u, 0xC0);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
