/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "party.h"






#define ROUNDUP(x) ((x) == (f32)(s32)(x) ? (x) : ((x) > 0.0f ? (f32)((s32)(x) + 1) : (f32)~(s32)(-x)))

s32 PtyManaEggSpellCost(PtyUnit *arg0, s32 i, s32 j) {
    PtyManaEgg *e;
    s16 *p;
    s16 v;

    e = &ptyPartyData->eggs[i];
    p = (s16 *)ptyMoveTable[e->spells[j]].levels;
    if (e->spellUpgrades[j] & 1) {
        p += 12;
    }
    v = *p;
    return ROUNDUP(v * arg0->mgcMpRate);
}
