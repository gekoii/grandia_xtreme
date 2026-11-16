#include "common.h"
#include "main/n03.h"

s32 SysParamSetMode(s32 arg0) {
    sysParamMode = arg0;
    return 1;
}

s32 SysParamSet(s32 arg0, s32 arg1) {
    sysParams.unk0 = arg0;
    sysParams.unk4 = arg1;
    return 1;
}
