#include "common.h"
#include "snd.h"
#include "protos/main.h"

s32 SndStreamPlayTracks(s32 arg0, s32 arg1) {
    s32 i = SndStreamPlay(arg0, 10, 100);
    if (i < 0) {
        return i;
    }
    sndBgmChannels[i].trackMask = arg1;
    return i;
}
