#include "common.h"
#include "party.h"

s32 ItemKeyRemove(s16 arg0) {
    s32 i;

    for (i = 0; i < 24; i++) {
        if (ptyPartyData->keyItems[i].id == arg0) {
            ptyPartyData->keyItems[i].id = 0;
            ptyPartyData->keyItems[i].count = 0;
            return 0;
        }
    }
    return -1;
}
