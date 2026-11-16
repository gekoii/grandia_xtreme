#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"
#include "battle/n34.h"
#include "protos/battle.h"

extern char dbgBtlParamRegRaceFmt[];
extern char dbgBtlParamDefRaceFmt[];

void DbgBtlCharParamRacePage(s32 idx) {
    PtyUnit *st = ((BtlUnit *)((idx << 9) + (u32)btlUnits))->params; /* integer index: the shift is scheduled ahead of the base address */
    s32 i;
    s32 j;

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlParamRegRaceFmt, st->regRace[0]);
    for (i = 1; i < 13; i++) {
        DbgPrintf(11, i + 4, dbgBtlFloatFmt, st->regRace[i]);
    }
    DbgPrintf(0x16, 4, dbgBtlParamDefRaceFmt, st->defRace[0]);
    for (j = 1; j < 13; j++) {
        DbgPrintf(0x1F, j + 4, dbgBtlIntFmt, st->defRace[j]);
    }
}
