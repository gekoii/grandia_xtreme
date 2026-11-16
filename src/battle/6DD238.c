#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"
#include "battle/n34.h"
#include "protos/battle.h"

extern char dbgBtlParamMagicRateFmt[];
extern char dbgBtlParamMagicPowFmt[];

void DbgBtlCharParamMagicPage(s32 idx) {
    PtyUnit *st = ((BtlUnit *)((idx << 9) + (u32)btlUnits))->params; /* integer index: the shift is scheduled ahead of the base address */
    s32 i;
    s32 j;

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlParamMagicRateFmt, st->magicRate[0]);
    for (i = 1; i < 9; i++) {
        DbgPrintf(13, i + 4, dbgBtlFloatFmt, st->magicRate[i]);
    }
    DbgPrintf(0x16, 4, dbgBtlParamMagicPowFmt, st->magicPow[0]);
    for (j = 1; j < 9; j++) {
        DbgPrintf(0x20, j + 4, dbgBtlIntFmt, st->magicPow[j]);
    }
}
