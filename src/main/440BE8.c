#include "common.h"
#include "main/n06.h"
#include "protos/main.h"


s32 SndStreamPlay(s32 id, s32 arg1, s32 arg2) {
    s32 idx;

    if (sndBgmEnabled == 0) {
        return -1;
    }
    idx = sndBgmCurChannel;
    if (sndBgmChannels[0].state != 0 && sndBgmChannels[0].id == id && sndBgmChannels[0].reqStop == 0) {
        return -1;
    }
    if (sndBgmChannels[1].state != 0 && sndBgmChannels[1].id == id && sndBgmChannels[1].reqStop == 0) {
        return -1;
    }
    sndBgmChannels[idx].index = id;
    sndBgmChannels[idx].id = id;
    sndBgmChannels[idx].state = 1;
    sndBgmChannels[idx].unk4 = 1;
    sndBgmChannels[idx].handle = idx + 1;
    sndBgmChannels[idx].reqStop = 0;
    sndBgmChannels[idx].volume = 0xFF;
    sndBgmChannels[idx].fade = 0;
    sndBgmChannels[idx].unk1C = 0;
    sndBgmChannels[idx].fadeState = 0;
    sndBgmChannels[idx].trackMask = 0;
    sndBgmChannels[idx].unk0 = VuRandInt(0xB4);
    return idx;
}
