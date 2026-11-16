#include "common.h"
#include "battle/misc.h"

void BtlMotionSeSetMuteRange(s32 arg0, s32 arg1) {
    btlMotionSeMuteFirst = arg0;
    btlMotionSeMuteLast = arg1;
}
