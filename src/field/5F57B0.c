#include "common.h"
#include "party.h"
#include "field/menu.h"

void ItemCharBookSlotGetType(s16 arg0, s16 arg1) {
    s16 id;
    s32 v;

    id = ptyCharData[arg0].slots[arg1];
    if (id == -1) {
        itemWork->bookSlotType = 9;
        return;
    }
    v = ptySkillBookTypeTable[ptyItemTable[ptyPartyData->books[id].itemId].subtype].type;
    itemWork->bookSlotType = v;
}
