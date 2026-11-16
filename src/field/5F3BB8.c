#include "common.h"
#include "party.h"
#include "field/n19.h"
#include "protos/field.h"

s16 ItemEggAdd(s16 id) {
    s32 i;
    s32 n = 0;
    s32 t;
    s32 k;
    s16 lo;
    s32 v;
    s8 kind;

    for (i = 0; i < 24; i++) {
        if (ptyPartyData->eggs[i].itemId != 0) {
            if (ptyPartyData->eggs[i].owner == -1 || PtyCharIsJoined(ptyPartyData->eggs[i].owner) >= 0) {
                n++;
            }
        }
    }
    if (n >= itemEggCapacity) {
        return -1;
    }
    for (i = 0; i < 24; i++) {
        if (ptyPartyData->eggs[i].itemId == 0) {
            t = id - 0x12C;
            lo = ptyManaEggTable[t].minMp;
            v = VuRandInt(ptyManaEggTable[t].maxMp - lo) + lo;
            kind = 0;
            if (t == 0x1D) kind = 1;
            if (t == 0x1E) kind = 2;
            ptyPartyData->eggs[i].itemId = id;
            ptyPartyData->eggs[i].type = t;
            ptyPartyData->eggs[i].owner = -1;
            ptyPartyData->eggs[i].maxMp = v;
            ptyPartyData->eggs[i].mp = v;
            ptyPartyData->eggs[i].unk1A = kind;
            ItemEggChartSetKnown(t);
            for (k = 0; k < 6; k++) {
                ptyPartyData->eggs[i].spells[k] = ptyManaEggTable[t].spells[k];
                ptyPartyData->eggs[i].spellUpgrades[k] = 0;
            }
            return i;
        }
    }
    return -1;
}
