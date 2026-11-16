#include "common.h"
#include "party.h"

void ItemSetShopSeen(s16 arg0) {
    ptyPartyData->itemFlags[arg0] |= 2;
}
