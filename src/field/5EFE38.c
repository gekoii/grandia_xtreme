#include "common.h"
#include "party.h"

s32 ItemIsShopSeen(s16 arg0) {
    s32 t = ptyPartyData->itemFlags[arg0] & 2;
    return t != 0;
}
