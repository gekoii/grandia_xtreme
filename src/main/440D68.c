#include "common.h"
#include "snd.h"

s32 SndBgmGetState(s32 arg0) {
    return sndBgmChannels[arg0].state;
}
