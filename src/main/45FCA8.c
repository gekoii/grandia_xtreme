#include "common.h"
#include "snd.h"

s32 SndStrmTell(u32 arg0) {
    if (arg0 >= 4U) {
        return -2;
    }
    if (sndStrmSlots[arg0].busy != 0) {
        return sndStrmSlots[arg0].pos;
    }
    return -2;
}
