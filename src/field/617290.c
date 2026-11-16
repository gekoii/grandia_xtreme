#include "common.h"
#include "snd.h"
#include "field/n22.h"

s32 FldSndStreamGetState(void) {
    if (fldSndStreamHandle == 0xFFFFFFFF) {
        return 0;
    }
    return sndBgmChannels[fldSndStreamHandle].state;
}
