#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemBookDiscard(u16 idx) {
    if (ptyPartyData->books[idx].itemId == 0) {
        return -1;
    }
    if (ptyPartyData->books[idx].owner != -1) {
        ItemCharUnequipOwned(ptyPartyData->books[idx].owner, 1, idx);
    }
    ptyPartyData->books[idx].itemId = 0;
    ptyPartyData->books[idx].owner = -1;
    ItemBookListBuild(0);
    return 0;
}
