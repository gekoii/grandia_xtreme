#include "common.h"
#include "party.h"
#include "field/n19.h"
#include "protos/field.h"

s16 ItemEggFindFreeSlot(void) {
    s32 i;
    s32 n = 0;

    for (i = 0; i < 24; i++) {
        if (ptyPartyData->eggs[i].itemId != 0) {
            if (ptyPartyData->eggs[i].owner == -1 || PtyCharIsJoined(ptyPartyData->eggs[i].owner) >= 0) {
                n++;
            }
        }
    }
    if (n >= itemEggCapacity) {
        return -1;
    }
    for (i = 0; i < 24; i++) {
        if (ptyPartyData->eggs[i].itemId == 0) {
            return i;
        }
    }
    return -1;
}
