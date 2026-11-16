#include "common.h"
#include "sys.h"
#include "protos/main.h"



void SysProgApplyRequests(void) {
    s32 i;
    u8 v;
    u8 save;

    for (i = 0; i < 8; i++) {
        if (sysState.active[i] != sysState.requested[i]) {
            sysState.active[i] = sysState.requested[i];
            if (i == 0) {
                v = sysState.active[0];
                if (v == 3) {
                    MemHeapReset(sysState.battleMagicHeap);
                    save = sysState.program;
                    sysState.program = v;
                    SysProgCall(2, 0, 0);
                    sysState.program = save;
                }
            }
        }
    }
}
