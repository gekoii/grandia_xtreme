#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemBagRemove(u16 arg0, u8 arg1) {
    s16 id = ptyPartyData->bag[arg0].id;
    s32 n;

    if (id == 0) {
        return -1;
    }
    switch (ptyItemTable[id].type) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 13:
        for (n = 0; n < arg1; n++) {
            ptyPartyData->bag[arg0].count--;
            if (ptyPartyData->bag[arg0].count == 0) {
                ptyPartyData->bag[arg0].id = 0;
            }
        }
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        ptyPartyData->bag[arg0].id = 0;
        ptyPartyData->bag[arg0].count = 0;
        break;
    }
    ItemBagListBuild(0);
    return 0;
}
