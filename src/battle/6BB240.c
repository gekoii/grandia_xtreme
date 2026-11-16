#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"


void BtlSchedKoDone(s32 n) {
    s32 m;

    m = btlKoPendingMask & ~(1 << n);
    btlKoPendingMask = m;
    if (n < 4 && m == 0) {
        SndBgmFadeVolume(btlBgmHandle.b, 0x96, 100.0f);
        BtlCamSetMode(1, 0x80, 0, 0);
    }
}
