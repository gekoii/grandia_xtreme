#include "common.h"
#include "protos/main.h"


s32 SysWaitFrame(s32 arg0) {
    SndSeBankLoadUpdate();
    SndBgmUpdate();
    FileReadUpdate(0);
    SndSeUpdate();
    SndIopPoll();
    return GsVsyncFlip(0);
}
