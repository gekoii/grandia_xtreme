#include "common.h"
#include "battle/unit.h"

void BtlSchedKoBegin(void) {
    BtlUnit *u;
    s32 i;

    btlActingUnit = 0xFE;
    u = btlUnits;
    for (i = 0; i < 16; i++, u++) {
        if ((u32)(u->state - 1) < 4) {
            u->flags |= 8;
        }
    }
    btlActRequestMask &= ~btlKoRequestMask;
    btlKoPendingMask = btlKoRequestMask;
    btlKoRequestMask = 0;
}
