#include "common.h"
#include "protos/main.h"
#include "main/n10.h"


s32 SndSoftVoiceAlloc(s32 prio) {
    s32 i;
    s32 best;
    s32 max;
    SndHandle *p;

    for (i = 0; i < 8; i++) {
        if (sndSoftVoices[i].state == 0) {
            return i;
        }
    }
    max = 50;
    best = -1;
    for (i = 0; i < 8; i++) {
        if (max < sndSoftVoices[i].unk24) {
            max = sndSoftVoices[i].unk24;
            best = i;
        }
    }
    if (best >= 0 && prio < max) {
        p = sndSoftVoices[best].handle;
        if (p->state == 3) {
            s32 r = SndHandleStop(p);
            if (r < 0) {
                return -2;
            }
        }
        return best;
    }
    for (i = 0; i < 8; i++) {
        if (sndSoftVoices[i].state == 2) {
            return i;
        }
    }
    return -2;
}
