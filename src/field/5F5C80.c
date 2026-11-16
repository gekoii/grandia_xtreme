#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemCharSlotListBuild(s16);

s32 ItemCharUnequipSkill(s16 arg0, s16 arg1, s16 arg2) {
    s16 *p;
    s16 id;

    if (arg0 >= 1000) {
        arg0 -= 1000;
    }
    p = (s16 *)(arg0 * 0x84 + (u32)ptyCharData + 0x4A);
    p += (arg1 - ptyCharDefTable[arg0].eggSlots) * 3 + arg2;
    id = *p;
    *p = -1;
    if (id == -1) {
        return -1;
    }
    ptyPartyData->skills[id].owner = -1;
    if (ItemCharDropUnusableEquip(arg0)) {
        ItemCharSlotListBuild(arg0);
    }
    return 0;
}
