#include "common.h"
#include "snd.h"



void SndBgmFadeOut(s32 i, s32 arg1) {
    sndBgmChannels[i].reqStop = 1;
    sndBgmChannels[i].volume = 0;
    if ((u32)sndBgmChannels[0].state >= 12 && (u32)sndBgmChannels[1].state >= 12) {
        sndBgmChannels[i].fade = 0;
    } else if ((u32)sndBgmChannels[i].state < 12) {
        sndBgmChannels[i].fade = 0;
    } else {
        sndBgmChannels[i].fade = arg1;
    }
}
