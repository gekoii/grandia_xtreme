#include "common.h"
#include "party.h"

s32 ItemStorageAdd(s16 arg0, s16 arg1) {
    if (arg0 >= 0x800) {
        return -1;
    }
    if (ptyPartyData->storage[arg0] + arg1 > 100) {
        ptyPartyData->storage[arg0] = 100;
    } else {
        ptyPartyData->storage[arg0] += arg1;
    }
    return 0;
}
