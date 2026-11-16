#include "common.h"
#include "protos/main.h"


void SndSetEffect(s32 arg0) {
    SndControl(0x10, arg0);
}
