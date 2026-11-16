#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

extern s8 sndHandleStopLock;

s32 SndHandleStop(SndHandle *arg0) {
    if (sndHandleStopLock != 0) {
        return -11;
    }
    sndHandleStopLock = 1;
    if (arg0->state > 0) {
        switch (arg0->type) {
        case 5:
            if (arg0->state == 3) {
                sndSeVoiceTimer[arg0->voice] = 1;
            } else {
                sndSeVoiceState[arg0->voice] = 0;
            }
            arg0->state = 0;
            break;
        case 10:
            sndSoftVoices[arg0->voice].state = 2;
            arg0->state = 0;
            break;
        case 4:
            if (SndIopCmdPush(0x228, arg0->chan, 0, 0) == 0) {
                sndHandleStopLock = 0;
                return -11;
            }
            sndStrmSlots[arg0->chan + 2].busy = 0;
            arg0->state = 0;
            break;
        default:
            arg0->state = 0;
            sndHandleStopLock = 0;
            return -4;
        }
        arg0->fadeSpeed = 0;
        arg0->curVol = 0;
        arg0->targetVol = 0;
    }
    sndHandleStopLock = 0;
    return 0;
}
