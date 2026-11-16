#include "common.h"
#include "sys.h"
#include "protos/main.h"
#include "main/n01.h"

void SysProgBuildDrawAll(void) {
    s32 i = 0;

    VuMtxPoolReset();
    GsPacketBufRewind(gsMainPacketBuf);
    for (; i < sysState.activeCount; i++) {
        sysState.program = sysState.active[i];
        SysProgCall(8, i, 0);
    }
}
