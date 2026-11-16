#include "common.h"
#include "party.h"

void ItemSetSold(s16 arg0) {
    ptyPartyData->itemFlags[arg0] |= 1;
}
