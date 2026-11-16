#include "common.h"
#include "party.h"

s32 ItemBagSlotCheck(u16 arg0) {
    s16 id = ptyPartyData->bag[arg0].id;

    if (id == 0) {
        return -1;
    }
    return (ptyItemTable[id].type == 14) ? -2 : 0;
}
