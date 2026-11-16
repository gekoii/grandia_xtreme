/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "sys.h"
#include "battle/n34.h"

extern s32 dbgBtlPcNo;
extern char dbgBtlPcSaveTitleFmt[];
extern char dbgBtlPcSaveNoFmt[];

void DbgBtlPcSavePage(void) {
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlPageSelect(&padPort1, 1);
        if (sysState.dbgCursor == 0) {
            dbgBtlPcNo = (dbgBtlPcNo + DbgEditValue(0, 0, 1, 1.0f) < 0.0f) ? 7
                       : (dbgBtlPcNo + DbgEditValue(0, 0, 1, 1.0f) > 7.0f) ? 0
                       : dbgBtlPcNo + DbgEditValue(0, 0, 1, 1.0f);
        }
    }
    switch (sysState.dbgPage) {
    case 0:
        DbgBtlPcSaveStatsPage(dbgBtlPcNo);
        break;
    case 1:
        DbgBtlPcSaveEquipPage(dbgBtlPcNo);
        break;
    }
    DbgPrintf(2, 2, dbgBtlPcSaveTitleFmt, dbgBtlColor1Str, dbgBtlColor0Str, sysState.dbgPage + 1);
    DbgPrintf(2, 3, dbgBtlPcSaveNoFmt, dbgBtlPcNo);
}
