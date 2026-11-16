#include "common.h"
#include "field/n26.h"
#include "sys.h"

void FldPlayTimeSet(s32 arg0) {
    fldPlayTimeStartFrame = sysLoopFrameCount;
    fldPlayTimeBase = arg0;
}
