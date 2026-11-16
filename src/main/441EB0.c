#include "common.h"
#include "protos/main.h"
#include "snd.h"


void SndPrepareWait(s32 arg0) {
    SndPrepareReq req;
    req.handle = 0x400;
    req.index = 0x5DC;
    req.unkC = arg0;
    req.unk8 = 0;
    while (SndPrepare(&req) < 0) {
        SysWaitFrame(0);
    }
}
