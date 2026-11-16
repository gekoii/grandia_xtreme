#include "common.h"
#include "party.h"

s32 ItemKeyAdd(s16 arg0) {
    s32 i;

    for (i = 0; i < 24; i++) {
        if (ptyPartyData->keyItems[i].id == 0) {
            ptyPartyData->keyItems[i].id = arg0;
            ptyPartyData->keyItems[i].count = 1;
            return 0;
        }
    }
    return -1;
}
