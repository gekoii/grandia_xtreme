#include "common.h"
#include "party.h"

s32 ItemBagRemoveById(s16 arg0) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (ptyPartyData->bag[i].id == arg0) {
            ptyPartyData->bag[i].id = 0;
            ptyPartyData->bag[i].count = 0;
            return 0;
        }
    }
    return -1;
}
