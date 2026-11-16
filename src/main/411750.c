/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "party.h"
#include "protos/main.h"






s32 PtyManaEggAdd(s32 arg0) {
    PtyManaEgg *slot;
    PtyManaEggType *d;
    s32 i;
    s32 j;
    s32 k;
    f32 f;

    slot = ptyPartyData->eggs;
    for (i = 0; i < 24; i++, slot++) {
        if (slot->itemId <= 0) {
            break;
        }
    }
    if (i >= 24) {
        return -1;
    }
    slot->itemId = arg0;
    slot->type = ptyItemTable[arg0].subtype;
    slot->owner = -1;
    d = &ptyManaEggTable[slot->type];
    slot->maxMp = slot->mp = (f32)(d->maxMp - d->minMp) * ((f32)(rand() >> 15) * (1.0f / 65536.0f)) + 0.5f + (f32)d->minMp;
    for (j = 0; j < 6; j++) {
        slot->spells[j] = d->spells[j];
        slot->spellUpgrades[j] = 0;
        for (k = 0; k < 7; k++) {
            f = d->upgradeChance[j][k] * 0.01f;
            if (f > 0.0f && (f32)(rand() >> 15) * (1.0f / 65536.0f) <= f) {
                slot->spellUpgrades[j] |= 1 << k;
            }
        }
    }
    return i;
}
