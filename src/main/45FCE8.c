#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndStrmGetReady(u32, s32);

s32 SndStrmClose(u32 arg0) {
    if (arg0 < 4U) {
        if (sndStrmSlots[arg0].busy != 0) {
            if (SndIopCmdPush(0x120, arg0, 0, 0) == 0) {
                return -1;
            }
            sndStrmFetchSlot = -0x10;
            sndStrmSlots[arg0].busy = 0;
            sndStrmSlots[arg0].size = 0;
            return SndStrmGetReady(arg0, 0);
        }
    }
    return -2;
}
