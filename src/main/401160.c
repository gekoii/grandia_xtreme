#include "common.h"
#include "sys.h"




void SysProgRegister(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    sysProgramTable[sysState.program].unk14 = arg0;
    sysProgramTable[sysState.program].unk18 = arg1;
    sysProgramTable[sysState.program].unk1C = arg2;
    sysProgramTable[sysState.program].update = arg3;
    sysProgramTable[sysState.program].draw = arg5;
    sysProgramTable[sysState.program].endFrame = arg6;
    sysProgramTable[sysState.program].debugMenu = arg7;
    sysProgramTable[sysState.program].buildDraw = arg4;
}
