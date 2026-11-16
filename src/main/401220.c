#include "common.h"
#include "sys.h"
#include "m2c_macros.h"


s32 SysIsDebugMode(void) {
    return sysDebugMode;
}

void SysSetDebugMode(void) {
    sysDebugMode = 1;
}

u8 SysIsBattleBoot(void) {
    return sysBattleBoot;
}
