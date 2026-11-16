#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemEggDiscard(s16 id) {
    s32 flag = 1;

    if (id >= 1000) {
        flag = 0;
        id -= 1000;
    }
    if (ptyPartyData->eggs[id].itemId == 0) {
        return -1;
    }
    if (ptyPartyData->eggs[id].owner != -1) {
        ItemCharUnequipOwned(ptyPartyData->eggs[id].owner, 0, id);
    }
    ptyPartyData->eggs[id].itemId = 0;
    ptyPartyData->eggs[id].owner = -1;
    if (flag) {
        ItemEggListBuild(0);
    }
    return 0;
}
