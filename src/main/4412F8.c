#include "common.h"
#include "protos/main.h"


void SndSetOutputMode(s32 arg0) {
    SndControl(0x100, &arg0);
}
