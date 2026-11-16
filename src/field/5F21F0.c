#include "common.h"
#include "party.h"



void PtySetShopPoints(u32 arg0) {
    if (arg0 > 999999) {
        arg0 = 999999;
    }
    ptyPartyData->shopPoints = arg0;
}
