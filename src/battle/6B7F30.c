#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"
#include "battle/n31.h"
#include "pad.h"
#include "sys.h"

extern char dbgEnvFlareTitleFmt[];
extern char dbgEnvFlareTitleColorStr[];
extern char dbgEnvFlareColor0Str[];
extern char dbgEnvFlareAlphaFmt[];
extern char dbgEnvFlareDistFmt[];
extern char dbgEnvFlareAngleXFmt[];
extern char dbgEnvFlareAngleYFmt[];
extern char dbgEnvFlareColor4Str[];
extern char dbgEnvFlarePosXFmt[];
extern char dbgEnvFlarePosYFmt[];
extern char dbgEnvFlarePosZFmt[];

void DbgEnvLensFlarePage(void) {
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 3);
        switch (sysState.dbgCursor) {
        case 0:
            envFlareAlpha += DbgEditValue(2, -3, 1, 0.1f);
            break;
        case 1:
            envFlareDistance += DbgEditValue(2, -1, 5, 100.0f);
            break;
        case 2:
            envFlareAngleX = envFlareAngleX + DbgEditValue(1, 0, 4, 256.0f);
            break;
        case 3:
            envFlareAngleY = envFlareAngleY + DbgEditValue(1, 0, 4, 256.0f);
            break;
        }
        EnvFlareCalcPos(EnvParamGet(btlEntryMapNo));
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, dbgEnvFlareTitleFmt, dbgEnvFlareTitleColorStr, dbgEnvFlareColor0Str);
    DbgPrintf(3, 3, dbgEnvFlareAlphaFmt, envFlareAlpha, envFlareIntensity);
    DbgPrintf(3, 4, dbgEnvFlareDistFmt, envFlareDistance);
    DbgPrintf(3, 5, dbgEnvFlareAngleXFmt, envFlareAngleX & 0xFFFF);
    DbgPrintf(4, 6, dbgEnvFlareAngleYFmt, envFlareAngleY & 0xFFFF);
    DbgPrintf(0, 0, dbgEnvFlareColor4Str);
    DbgPrintf(3, 7, dbgEnvFlarePosXFmt, envFlarePos[0]);
    DbgPrintf(4, 8, dbgEnvFlarePosYFmt, envFlarePos[1]);
    DbgPrintf(4, 9, dbgEnvFlarePosZFmt, envFlarePos[2]);
    DbgPrintf(0, 0, dbgEnvFlareColor0Str);
}
