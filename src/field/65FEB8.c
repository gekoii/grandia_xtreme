#include "common.h"
#include "field/n26.h"
#include "sys.h"

s32 FldPlayTimeGet(void) {
    return (sysLoopFrameCount - fldPlayTimeStartFrame) + fldPlayTimeBase;
}
