#include "common.h"
#include "m2c_macros.h"

extern s32 chrTalkWork;

s32 ChrTalkIsActive(void) {
    return chrTalkWork != 0;
}
