#include "common.h"
#include "party.h"




s32 PtyManaEggGetSpellLevel(s32 arg0, s32 arg1, s32 arg2) {
    PtyManaEgg *e = &ptyPartyData->eggs[arg0];
    s32 ret;
    s32 flags;

    ret = (s32)ptyMoveTable[e->spells[arg1]].levels;
    flags = e->spellUpgrades[arg1] & arg2;
    if (flags != 0) {
        ret += 0x18;
        if (flags & 0x40) {
            ret += 0x18;
            if (flags & 6) {
                ret += 0x18;
            }
        }
    }
    return ret;
}
