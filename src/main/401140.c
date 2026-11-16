#include "common.h"
#include "sys.h"
#include "protos/main.h"



void SysProgDebugMenu(void) {
    sysState.program = sysState.active[0];
    SysProgCall(7, 0, 0);
}
