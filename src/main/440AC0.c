#include "common.h"
#include "main/n06.h"
#include "protos/main.h"


s32 SndBgmPlay(s32 id, s32 arg1) {
    s32 cur;
    s32 other;

    if (sndBgmEnabled == 0) {
        return -1;
    }
    if (sndBgmChannels[0].state != 0 && sndBgmChannels[0].id == id && sndBgmChannels[0].reqStop == 0) {
        return -1;
    }
    if (sndBgmChannels[1].state != 0 && sndBgmChannels[1].id == id && sndBgmChannels[1].reqStop == 0) {
        return -1;
    }
    cur = sndBgmCurChannel;
    other = cur ^ 1;
    sndBgmChannels[cur].state = 1;
    sndBgmChannels[cur].index = id;
    sndBgmChannels[cur].id = id;
    sndBgmChannels[cur].unk4 = 0;
    sndBgmChannels[cur].handle = cur + 1;
    sndBgmChannels[cur].reqStop = 0;
    sndBgmChannels[cur].volume = 0xFF;
    sndBgmChannels[cur].fade = arg1;
    sndBgmChannels[cur].unk1C = 0;
    sndBgmChannels[cur].fadeState = 0;
    sndBgmChannels[cur].trackMask = 0;
    if (sndBgmChannels[other].state != 0) {
        SndBgmFadeOut(other, arg1);
    }
    sndBgmCurChannel ^= 1;
    return cur;
}
