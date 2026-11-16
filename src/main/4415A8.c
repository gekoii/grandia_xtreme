#include "common.h"
#include "protos/main.h"

void SndSeBankLoad(char *arg0, s32 arg1, s32 arg2, s32 arg3) {
    SndSeBankLoadStart(arg0, arg1, arg2, arg3);
    while (SndSeBankLoadGetState() != -1) {
        SysWaitFrame(0);
    }
    SysWaitFrame(0);
}
