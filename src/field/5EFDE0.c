#include "common.h"
#include "party.h"

s32 ItemIsSold(s16 arg0) {
    if (arg0 == 0) {
        return 1;
    }
    return ptyPartyData->itemFlags[arg0] & 1;
}
