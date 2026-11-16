#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndSetPause(s32 arg0) {
    if (arg0 == 1) {
        if (SndIopCmdPush(0x240, 0, 0, 0) == 0) {
            return -0xB;
        }
        sndPaused = arg0;
    } else {
        if (SndIopCmdPush(0x241, 0, 0, 0) == 0) {
            return -0xB;
        }
        sndPaused = 0;
    }
    return 0;
}
