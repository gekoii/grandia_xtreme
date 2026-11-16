#include "common.h"
#include "party.h"

s16 ItemBagAdd(s16 arg0, s16 arg1) {
    s32 i;
    s32 free = -1;

    for (i = 0; i < 30; i++) {
        if (ptyPartyData->bag[i].id == 0 || ptyPartyData->bag[i].count == 0) {
            if (free == -1) {
                free = i;
            }
        } else if (ptyPartyData->bag[i].id == arg0 && ptyItemTable[arg0].type != 13) {
            if (ptyPartyData->bag[i].count < arg1) {
                ptyPartyData->bag[i].count++;
                return 0;
            }
            if (arg1 != 1) {
                return -1;
            }
        }
    }
    if (free != -1) {
        ptyPartyData->bag[free].id = arg0;
        ptyPartyData->bag[free].count = 1;
        return 0;
    }
    return -1;
}
