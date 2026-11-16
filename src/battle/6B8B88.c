#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"
#include "pad.h"
#include "sys.h"

extern char dbgEnvFieldTitleFmt[];
extern char dbgEnvFieldTitleColorStr[];
extern char dbgEnvFieldColor0Str[];
extern char dbgEnvFieldClipStr[];
extern char dbgEnvFieldChkDispStr[];

void DbgEnvFieldPage(void) {
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 1);
        switch (sysState.dbgCursor) {
        case 0:
            if (padPort1.repeat & 0xA000) {
                envStageClipOn ^= 1;
            }
            break;
        case 1:
            if (padPort1.repeat & 0xA000) {
                envStageZoneDisp ^= 1;
            }
            break;
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, dbgEnvFieldTitleFmt, dbgEnvFieldTitleColorStr, dbgEnvFieldColor0Str);
    DbgBtlPrintOnOff(2, 3, dbgEnvFieldClipStr, envStageClipOn);
    DbgBtlPrintOnOff(2, 4, dbgEnvFieldChkDispStr, envStageZoneDisp);
}
