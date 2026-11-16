/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"
#include "battle/n31.h"
#include "pad.h"
#include "sys.h"

extern char dbgEnvShadowTitleFmt[];
extern char dbgEnvShadowRFmt[];
extern char dbgEnvShadowGFmt[];
extern char dbgEnvShadowBFmt[];
extern char dbgEnvShadowAFmt[];
extern char dbgEnvShadowCallbackFmt[];

void DbgEnvShadowPage(void) {
    s32 i;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlPageSelect(&padPort1, 1);
        if (sysState.dbgPage == 0) {
            DbgBtlLineSelect(&padPort1, 3);
            envLight.shadowColor[sysState.dbgCursor] += DbgEditValue(0, 0, 3, 1.0f);
            envLight.shadowColor[sysState.dbgCursor] = (envLight.shadowColor[sysState.dbgCursor] < -255.0f) ? -255.0f
                : (envLight.shadowColor[sysState.dbgCursor] > 255.0f) ? 255.0f : envLight.shadowColor[sysState.dbgCursor];
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, dbgEnvShadowTitleFmt, dbgEnvTitleColorStr, dbgEnvColor0Str, sysState.dbgPage + 1);
    switch (sysState.dbgPage) {
    case 0:
        DbgPrintf(3, 3, dbgEnvShadowRFmt, envLight.shadowColor[0]);
        DbgPrintf(3, 4, dbgEnvShadowGFmt, envLight.shadowColor[1]);
        DbgPrintf(3, 5, dbgEnvShadowBFmt, envLight.shadowColor[2]);
        DbgPrintf(3, 6, dbgEnvShadowAFmt, envLight.shadowColor[3]);
        break;
    case 1:
        for (i = 0; i < 16; i++) {
            DbgPrintf(3, i + 3, dbgEnvShadowCallbackFmt, envShadowPreCb[i], envShadowDrawCb[i], envShadowCbArg[i]);
        }
        break;
    }
}
