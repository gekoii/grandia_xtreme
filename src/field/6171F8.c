#include "common.h"
#include "snd.h"
#include "field/n22.h"

s32 FldSndStreamGetPlayPos(void) {
    u32 t;
    if (fldSndStreamHandle == 0xFFFFFFFF || (t = sndBgmChannels[fldSndStreamHandle].state) == 0xFFFFFFFF || t == 0) {
        return -1;
    }
    if (t != 12) {
        return 0;
    }
    return sndBgmChannels[fldSndStreamHandle].status.kTime;
}
