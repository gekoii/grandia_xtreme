#include "common.h"
#include "party.h"

s32 ItemCharSetBook(s16 arg0, s16 arg1, s16 arg2) {
    ptyCharData[arg0].slots[arg1] = arg2;
    ptyPartyData->books[arg2].owner = arg0;
    return 0;
}
