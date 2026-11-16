#include "common.h"
#include "battle/n33.h"

void BtlHitSetNextCallback(s32 arg0, s32 arg1) {
    btlHitNextCallback = (void (*)(s32, s32, BtlUnit *))arg0;
    btlHitNextCallbackArg = arg1;
}

void BtlHitSetNextFlags(s32 arg0) {
    btlHitNextFlags = arg0;
}

s32 BtlHitGetNextFlags(void) {
    return btlHitNextFlags;
}
