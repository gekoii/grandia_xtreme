#include "common.h"

#include "battle/unit.h"
#include "battle/n33.h"

void BtlHitQueueInit(void) {
    s32 i;

    btlHitQueueCount = 0;
    for (i = 0; i < 16; i++) {
        btlKnockbackAccum[i] = 0;
        btlHitDamageAccum[i] = -1;
    }
    btlHitNextVictimHandler = NULL;
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(btlHitNextSrcPos) : "memory");
    btlHitNextSrcPos[3] = 0.0f;
    btlHitNextCallback = NULL;
    btlHitNextFlags = 0;
    for (i = 0; i < 16; i++) {
        btlReviveHpMode[i] = 0;
    }
}
