#include "common.h"
#include "party.h"
#include "field/n19.h"
#include "protos/field.h"

s16 ItemBookAdd(s16 arg0) {
    s32 i;
    s32 n = 0;

    for (i = 0; i < 24; i++) {
        if (ptyPartyData->books[i].itemId != 0) {
            if (ptyPartyData->books[i].owner == -1 || PtyCharIsJoined(ptyPartyData->books[i].owner) >= 0) {
                n++;
            }
        }
    }
    if (n >= itemBookCapacity) {
        return -1;
    }
    for (i = 0; i < 24; i++) {
        if (ptyPartyData->books[i].itemId == 0) {
            ptyPartyData->books[i].itemId = arg0;
            ptyPartyData->books[i].owner = -1;
            return i;
        }
    }
    return -1;
}
