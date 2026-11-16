#include "common.h"
#include "battle/n33.h"

void BtlHitSetNextVictimHandler(s32 arg0) {
    btlHitNextVictimHandler = (void (*)(BtlUnit *))arg0;
}

s32 BtlHitGetNextVictimHandler(void) {
    return (s32)btlHitNextVictimHandler;
}
