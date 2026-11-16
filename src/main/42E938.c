#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

s32 SysParamInit(s32 arg0, s32 arg1) {
    u32 i; u8 *p = (u8 *)&sysParams;

    for (i = 0; i < sizeof(sysParams); i++) {
        p[i] = 0;
    }
    return SysParamSet(arg0, arg1) != 0;
}
