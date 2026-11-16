#include "common.h"
#include "main/n06.h"

void SndBgmSetEnabled(s32 arg0) {
    sndBgmEnabled = arg0 & 0xFF;
}
