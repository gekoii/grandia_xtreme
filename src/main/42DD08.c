#include "common.h"
#include "pad.h"
#include "m2c_macros.h"


s32 PadIsInputLocked(void) {
    return padInputLocked;
}

void PadSetInputLocked(s32 arg0) {
    padInputLocked = arg0 & 0xFF;
}

void PadSetVibEnabled(s32 arg0) {
    padVibEnabled = arg0 & 0xFF;
}
