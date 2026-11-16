#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemStorageDeposit(s16 arg0, s16 arg1) {
    s16 id = ptyPartyData->bag[arg0].id;

    if (id == 0) {
        return -1;
    }
    ptyPartyData->storage[id] += arg1;
    if (ptyPartyData->storage[id] > 100) {
        ptyPartyData->storage[id] = 100;
        return -1;
    }
    return ItemBagRemove(arg0, arg1);
}
