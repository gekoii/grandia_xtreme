#include "common.h"
#include "snd.h"

s32 SndStrmSeek(u32 arg0, s32 arg1) {
    if (arg0 >= 4U) {
        return -2;
    }
    if (sndStrmSlots[arg0].busy != 0) {
        if (sndStrmSlots[arg0].size < (arg1 << 0xB)) {
            return -4;
        }
        sndStrmSlots[arg0].pos = arg1;
        return arg1;
    }
    return -2;
}
