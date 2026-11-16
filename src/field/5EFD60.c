#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemBagSell(s16 arg0, u8 arg1) {
    s16 id;

    ItemBagRemove(arg0, arg1);
    id = ptyPartyData->bag[arg0].id;
    if (id != 0) {
        ItemSetSold(id);
        return 0;
    }
    return -1;
}
