#include "common.h"
#include "party.h"



void PtySetGold(u32 arg0) {
    if (arg0 > 99999999) {
        arg0 = 99999999;
    }
    ptyPartyData->gold = arg0;
}
