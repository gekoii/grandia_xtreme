#include "common.h"
#include "battle/n32.h"

void AiSpellAllowInit(void) {
    u8 i;

    aiSpellAllowTable[0] = 1;
    for (i = 1; i < 0x4A; i++) {
        aiSpellAllowTable[i] = 1;
    }
    for (; i < 0x80; i++) {
        aiSpellAllowTable[i] = 0;
    }
}
