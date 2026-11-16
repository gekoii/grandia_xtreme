#include "common.h"
#include "party.h"
#include "protos/field.h"

void ItemCharSlotListBuild(s16);

s16 ItemCharUnequipAllEggs(u16 a) {
    s32 ret;
    s32 i;
    s32 n;

    ret = 0;
    n = ptyCharDefTable[a].eggSlots;
    for (i = 0; i < n; i++) {
        if (ptyCharData[a].slots[i] != -1) {
            if (ItemCharUnequipEgg(a + 1000, i) == 0) {
                ret = 1;
            }
        }
    }
    ItemCharSlotListBuild(a);
    return ret;
}
