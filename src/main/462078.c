#include "common.h"
#include "protos/main.h"
#include "main/n10.h"


void SndUpdate(void) {
    SndHandle *e;
    s32 i;
    s32 v;

    e = sndHandles;
    if (sndInitDone == 0) {
        return;
    }
    if (sndPaused != 0) {
        return;
    }
    for (i = 0; i < 64; i++, e++) {
        if (e->state == 3) {
            e->timer++;
            if (e->duration > 0 && e->timer >= e->duration) {
                SndHandleStop(e);
            }
        }
        if (e->type == 4 && e->state > 0) {
            switch (e->state) {
            case 1:
                if ((u8)sndIopStatus->strmInfo[e->chan].state - 3 < 3U) {
                    e->state = 2;
                }
                break;
            case 2:
                if (e->voice == 0) {
                    e->voice = 1;
                    e->duration = sndIopStatus->strmInfo[e->chan].length;
                }
                if (e->unk7 != 0) {
                    if (SndIopCmdPush(0x218, e->chan, 0, 0) != 0) {
                        e->unk7 = 2;
                        e->state = 3;
                    }
                }
                break;
            default:
                if (sndIopStatus->strmInfo[e->chan].state == 0xB) {
                    e->state = -1;
                    sndStrmSlots[e->chan + 2].busy = 0;
                }
                break;
            }
        }
        if (e->state != 0) {
            v = e->curVol;
            if (v < e->targetVol) {
                v += sndFadeInStep[e->fadeSpeed];
                if (v > e->targetVol) {
                    v = e->targetVol;
                }
                if (SndHandleSetVolume(e, v) != 0) {
                    e->curVol = v;
                }
            }
            if (e->curVol > e->targetVol) {
                v = v * sndFadeOutRate[e->fadeSpeed] / 10000;
                if (v < e->targetVol) {
                    v = e->targetVol;
                }
                if (SndHandleSetVolume(e, v) != 0) {
                    e->curVol = v;
                }
            }
        }
    }
    for (i = 0; i < 24; i++) {
        if (sndSeVoiceState[i] < 0) {
            sndSeVoiceState[i]++;
        }
        if (sndSeVoiceTimer[i] > 0) {
            sndSeVoiceTimer[i]--;
            if (sndSeVoiceState[i] > 0 && sndSeVoiceTimer[i] == 0) {
                if (SndIopCmdPush(0x220, i, 0, 0) != 0) {
                    sndSeVoiceState[i] = -10;
                } else {
                    sndSeVoiceTimer[i] = 1;
                }
            }
        }
    }
}
