#include "common.h"
#include "battle/n33.h"

void BtlHitNextClear(void) {
    btlHitNextFlags = 0;
    btlHitNextCallback = NULL;
    btlHitNextSrcPosSet = 0;
    btlHitNextVictimHandler = NULL;
}
