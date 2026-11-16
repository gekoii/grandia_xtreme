#include "common.h"
#include "pad.h"



void PadVibrate(u8 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    if (padVibEnabled != 0) {
        if (arg1 != 1) {
            arg1 = 0;
        }
        arg0[0x23] = 1;
        if (arg1 == 0 && arg2 == 0) {
        padVibState.active = 0;
        padVibState.time = 0;
        padVibState.small = 0;
        padVibState.big = 0;
        padVibState.decay = 0;
        } else {
            padVibState.active = 1;
            padVibState.time = arg3;
            padVibState.small = arg1;
            if (padVibState.big < arg2) {
                padVibState.big = arg2;
            }
            padVibState.decay = 1;
        }
    }
}
