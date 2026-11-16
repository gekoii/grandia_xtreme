#include "common.h"
#include "sys.h"
#include "protos/main.h"



void SysProgEndFrameAll(void) {
    s32 i;

    for (i = 0; i < sysState.activeCount; i++) {
        sysState.program = sysState.active[i];
        SysProgCall(6, i, 0);
    }
}
