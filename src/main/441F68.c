#include "common.h"
#include "main/n06.h"

void SndSeSetRequestEnabled(s32 arg0) {
    sndSeRequestEnabled = arg0 & 0xFF;
}
