/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "sys.h"
#include "battle/n34.h"

extern char dbgBtlParamTitleFmt[];

void DbgBtlCharParamPage(void) {
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlPageSelect(&padPort1, 7);
        if (sysState.dbgCursor == 0) {
            dbgBtlCharNo = (dbgBtlCharNo + DbgEditValue(0, 0, 1, 1.0f) < 0.0f) ? 15
                       : (dbgBtlCharNo + DbgEditValue(0, 0, 1, 1.0f) > 15.0f) ? 0
                       : dbgBtlCharNo + DbgEditValue(0, 0, 1, 1.0f);
        }
    }
    switch (sysState.dbgPage) {
    case 0:
        DbgBtlCharParamBasePage(dbgBtlCharNo);
        break;
    case 1:
        DbgBtlCharParamEquipPage(dbgBtlCharNo);
        break;
    case 2:
        DbgBtlCharParamRacePage(dbgBtlCharNo);
        break;
    case 3:
        DbgBtlCharParamMagicPage(dbgBtlCharNo);
        break;
    case 4:
        DbgBtlCharParamResistPage(dbgBtlCharNo);
        break;
    case 5:
        DbgBtlCharParamAbilityPage(dbgBtlCharNo);
        break;
    case 6:
        DbgBtlCharParamGrowthPage(dbgBtlCharNo);
        break;
    case 7:
        DbgBtlCharParamKregPage(dbgBtlCharNo);
        break;
    }
    DbgPrintf(2, 2, dbgBtlParamTitleFmt, dbgBtlColor1Str, dbgBtlColor0Str, sysState.dbgPage + 1);
    DbgPrintf(2, 3, dbgBtlCharNoFmt, dbgBtlCharNo);
}
