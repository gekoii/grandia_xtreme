#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "pad.h"
#include "sys.h"
#include "battle/n34.h"


extern char dbgBtlIpStopTitleFmt[];
extern char dbgBtlIpStopLineFmt[];

void DbgBtlIpStopPage(void) {
    s32 i;
    s32 one;
    s32 on;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 0xF);
        if (padPort1.trigger & 0xA020) {
            btlDebugCfg.ipStopMask ^= 1 << sysState.dbgCursor;
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, dbgBtlIpStopTitleFmt, dbgBtlColor1Str, dbgBtlColor0Str);
    for (i = 0; i < 16; i++) {
        one = 1;
        on = 0;
        if ((btlDebugCfg.ipStopMask >> i) & 1) on = one;
        DbgPrintf(2, i + 3, dbgBtlIpStopLineFmt, i, on);
    }
}
