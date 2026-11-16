/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"
#include "battle/n31.h"
#include "pad.h"
#include "sys.h"

extern char dbgEnvContrastTitleFmt[];
extern char dbgEnvContrastAmbFmt[];
extern char dbgEnvContrastDirFmt[];

void DbgEnvContrastPage(void) {
    Pad *pad;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        DbgBtlPageSelect(pad, 0);
        DbgBtlLineSelect(pad, 1);
        switch (sysState.dbgCursor) {
        case 0:
            envContrastAmb += DbgEditValue(2, -3, 0, 0.1f);
            envContrastAmb = (envContrastAmb < 0.0f) ? 0.0f : (envContrastAmb > 1.0f) ? 1.0f : envContrastAmb;
            break;
        case 1:
            envContrastDir += DbgEditValue(2, -3, 0, 0.1f);
            envContrastDir = (envContrastDir < 0.0f) ? 0.0f : (envContrastDir > 1.0f) ? 1.0f : envContrastDir;
            break;
        }
        if (pad->repeat & 0xA000) {
            EnvLightCalc();
            EnvLightApply();
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, dbgEnvContrastTitleFmt, dbgEnvTitleColorStr, dbgEnvColor0Str, btlEntryMapNo, envParamIndex);
    DbgPrintf(3, 3, dbgEnvContrastAmbFmt, envContrastAmb);
    DbgPrintf(3, 4, dbgEnvContrastDirFmt, envContrastDir);
}
