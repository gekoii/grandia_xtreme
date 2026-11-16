#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemCanAdd(s16);

s32 ItemStorageWithdraw(s16 arg0, s16 arg1) {
    if (ItemCanAdd(arg0) == -1 || ptyPartyData->storage[arg0] < arg1) {
        return -1;
    }
    ptyPartyData->storage[arg0] -= arg1;
    if (ptyPartyData->storage[arg0] < 0) {
        ptyPartyData->storage[arg0] = 0;
    }
    return ItemAdd(arg0);
}
