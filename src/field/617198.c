#include "common.h"
#include "snd.h"
#include "field/n22.h"

s32 FldSndBgmGetPlayPos(void) {
    u32 t;
    if (fldSndBgmHandle == 0xFFFFFFFF || (t = sndBgmChannels[fldSndBgmHandle].state) == 0xFFFFFFFF || t == 0) {
        return -1;
    }
    if (t != 12) {
        return 0;
    }
    return sndBgmChannels[fldSndBgmHandle].status.kTime;
}
