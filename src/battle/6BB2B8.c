#include "common.h"
#include "battle/unit.h"

void BtlSchedActRequest(s32 arg0) {
    btlActRequestMask |= 1 << arg0;
}

void BtlSchedActCancel(s32 arg0) {
    btlActRequestMask &= ~(1 << arg0);
}

s32 BtlSchedFxActRequest(void) {
    if ((btlActingUnit != 0xFF) || (btlActRequestMask & 0x10000)) {
        return 0;
    }
    btlActRequestMask |= 0x10000;
    return 1;
}

void BtlSchedActDone(s32 arg0) {
    btlActDoneMask |= 1 << arg0;
}
