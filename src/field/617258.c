#include "common.h"
#include "snd.h"
#include "field/n22.h"

s32 FldSndBgmGetState(void) {
    if (fldSndBgmHandle == 0xFFFFFFFF) {
        return 0;
    }
    return sndBgmChannels[fldSndBgmHandle].state;
}
