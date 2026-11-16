/* rodata: 0x006740A8-0x006740B0 */
#include "common.h"
#include "protos/field.h"
#include "party.h"


s32 ItemEggSpellCanCast(s16 a, s16 b, s16 c) {
    u8 tbl[8] = { 0, 1, 1, 1, 0, 0, 1, 0 };
    s32 sel;
    s16 k;
    s16 lv;
    PtyMoveEffect *q;
    s8 t;
    s32 r;

    sel = PtyPartyGetSlot(a);
    if (ptyPartyData->eggs[b].itemId == 0) {
        return -2;
    }
    k = ptyPartyData->eggs[b].spells[c];
    if (k == 0) {
        return -2;
    }
    lv = ptyPartyData->eggs[b].mp;
    r = PtyManaEggSpellCost(&ptyPartyUnits[sel], b, c);
    q = ptyMoveTable[k].levels->effects;
    t = q->kind;
    if (lv < r) {
        return -3;
    }
    if (t >= 4) {
        if (t < 8 && tbl[t] == 0) {
            return -1;
        }
        if (q->statMask != 0) {
            return -1;
        }
        if (!(q->flags & 0x9810)) {
            return -1;
        }
    }
    return 0;
}
