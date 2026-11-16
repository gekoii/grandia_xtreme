#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"
#include "battle/n34.h"
#include "protos/battle.h"

extern char dbgBtlParamKregGoodFmt[];
extern char dbgBtlParamKregSpiFmt[];
extern char dbgBtlParamKregSealFmt[];
extern char dbgBtlParamKregOtherFmt[];
extern char dbgBtlParamKregPowDFmt[];
extern char dbgBtlParamKregPowUFmt[];
extern char dbgBtlParamExpSkl0Fmt[];
extern char dbgBtlParamExpSkl1Fmt[];

void DbgBtlCharParamKregPage(s32 idx) {
    PtyUnit *st = ((BtlUnit *)((idx << 9) + (u32)btlUnits))->params; /* integer index: the shift is scheduled ahead of the base address */
    s32 i;
    s32 j;

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlParamKregGoodFmt, st->kregGood);
    DbgPrintf(2, 5, dbgBtlParamKregSpiFmt, st->kregSpi);
    DbgPrintf(2, 6, dbgBtlParamKregSealFmt, st->kregSeal);
    DbgPrintf(2, 7, dbgBtlParamKregOtherFmt, st->kregOther);
    DbgPrintf(0x16, 4, dbgBtlParamKregPowDFmt, st->kregPowD[0]);
    for (i = 1; i < 5; i++) {
        DbgPrintf(0x21, i + 4, dbgBtlFloatFmt, st->kregPowD[i]);
    }
    DbgPrintf(0x16, 9, dbgBtlParamKregPowUFmt, st->kregPowU[0]);
    for (j = 1; j < 5; j++) {
        DbgPrintf(0x21, j + 9, dbgBtlFloatFmt, st->kregPowU[j]);
    }
    DbgPrintf(2, 8, dbgBtlParamExpSkl0Fmt, st->expSkl[0]);
    DbgPrintf(2, 10, dbgBtlParamExpSkl1Fmt, st->expSkl[1]);
}
