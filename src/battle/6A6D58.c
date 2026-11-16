#include "common.h"
#include "battle/misc.h"
#include "m2c_macros.h"

void BtlHudSetVisible(s32 arg0) {
    if (arg0 != 0) {
        btlHudHideFlags &= 0xFFFFEFFF;
        return;
    }
    btlHudHideFlags |= 0x1000;
}
