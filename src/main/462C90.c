#include "common.h"
#include "main/n10.h"

s32 SndAllocSeVoice(s32 arg0) {
    s32 i;
    s32 best;
    s32 idx;
    SndHandle *bp;
    SndHandle *e;

    for (i = 0; i < 24; i++) {
        if (sndSeVoiceState[i] == 0) {
            return i;
        }
    }
    e = sndHandles;
    bp = NULL;
    best = 0x7FFF;
    idx = -1;
    for (i = 0; i < 64; i++) {
        if (e->state != 0 && e->type == 5) {
            if (sndSeVoiceVol[e->voice] < best) {
                best = sndSeVoiceVol[e->voice];
                idx = e->voice;
                bp = e;
            }
        }
        e++;
    }
    if (idx >= 0) {
        bp->state = 0;
        sndSeVoiceTimer[idx] = 1;
        return idx;
    }
    for (i = 0; i < 24; i++) {
        if (sndSeVoiceState[i] < 0) {
            return i;
        }
    }
    return idx;
}
