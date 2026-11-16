#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "sys.h"

extern char btlStrHeapFree[];

void BtlUnitsPreDraw(void) {
    BtlUnit *u;
    s32 i;

    u = btlUnits;
    GphDrawListSet(btlPartyDrawList);
    GphDrawListClear(NULL);
    for (i = 0; i < 4; i++, u++) {
        if (u->state > 0) {
            BtlUnitPreDraw(u);
        }
    }
    GphDrawListSort();
    GphDrawListEnvMapCalcUv();
    GphDrawListSet(btlEnemyDrawList);
    GphDrawListClear(NULL);
    for (i = 0; i < 12; i++, u++) {
        if (u->state > 0) {
            BtlUnitPreDraw(u);
        }
    }
    GphDrawListSort();
    GphDrawListEnvMapCalcUv();
    if (SysIsDebugMode() != 0) {
        DbgPrintf(0x27, 2, btlStrHeapFree, MemGetLargestFree(), MemGetFreeTotal());
        MemHeapSelect(sysState.battleMagicHeap);
        DbgPrintf(0x27, 3, btlStrHeapFree, MemGetLargestFree(), MemGetFreeTotal());
        MemHeapSelect(sysState.battleHeap);
    }
}
