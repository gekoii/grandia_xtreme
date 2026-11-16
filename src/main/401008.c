#include "common.h"
#include "sys.h"
#include "protos/main.h"



void SysProgUpdateAll(void) {
    s32 i;

    for (i = 0; i < sysState.activeCount; i++) {
        sysState.program = *(i + sysState.active);
        SysProgCall(5, i, 0);
    }
}
