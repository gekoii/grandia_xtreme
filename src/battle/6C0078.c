#include "common.h"
#include "party.h"
#include "battle/unit.h"

/* Three parameters: 0x006C4410 passes a fourth (1) through a cast; the other callers pass three. */
PtyMoveLevelData *BtlSpellGetLevelData(BtlUnit *arg0, s32 j, s32 mask) {
    PtyManaEgg *e;
    s32 f;
    PtyMoveLevelData *p;

    e = &ptyPartyData->eggs[arg0->eggNo[j]];
    f = e->spellUpgrades[arg0->eggMagic[j]] & mask;
    p = ptyMoveTable[arg0->skillId].levels;
    if (f) {
        p++;
        if (f & 0x40) {
            p++;
            if (f & 6) {
                p++;
            }
        }
    }
    return p;
}
