#include "common.h"
#include "party.h"
#include "protos/field.h"

void ItemCharSlotListBuild(s16);

s16 ItemCharUnequipAllBooks(u16 a) {
    s32 ret;
    s32 i;

    ret = 0;
    for (i = ptyCharDefTable[a].eggSlots; i < 6; i++) {
        if (ptyCharData[a].slots[i] != -1) {
            if (ItemCharUnequipBook(a + 1000, i) == 0) {
                ret = 1;
            }
        }
    }
    ItemCharSlotListBuild(a);
    return ret;
}
