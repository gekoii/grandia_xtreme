#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"
#include "battle/n34.h"
#include "protos/battle.h"

extern char dbgBtlParamColRFmt[];
extern char dbgBtlParamColHFmt[];
extern char dbgBtlParamRunTimeFmt[];
extern char dbgBtlParamRegMgcIprFmt[];
extern char dbgBtlParamRegMgcAttrFmt[];
extern char dbgBtlParamRegIpFmt[];
extern char dbgBtlParamRegKbFmt[];
extern char dbgBtlParamBitCsfFmt[];
extern char dbgBtlParamRegStaFmt[];

void DbgBtlCharParamResistPage(s32 idx) {
    PtyUnit *st = ((BtlUnit *)((idx << 9) + (u32)btlUnits))->params; /* integer index: the shift is scheduled ahead of the base address */
    s32 i;
    s32 j;

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlParamColRFmt, st->colR);
    DbgPrintf(2, 5, dbgBtlParamColHFmt, st->colH);
    DbgPrintf(2, 6, dbgBtlParamRunTimeFmt, st->runTime);
    DbgPrintf(2, 7, dbgBtlParamRegMgcIprFmt, st->regMgcIpr);
    DbgPrintf(2, 8, dbgBtlParamRegMgcAttrFmt, st->regMgcAttr[0]);
    for (i = 1; i < 9; i++) {
        DbgPrintf(0xF, i + 8, dbgBtlFloatFmt, st->regMgcAttr[i]);
    }
    DbgPrintf(0x16, 4, dbgBtlParamRegIpFmt, st->regIp);
    DbgPrintf(0x16, 5, dbgBtlParamRegKbFmt, st->regKb);
    DbgPrintf(0x16, 6, dbgBtlParamBitCsfFmt, st->bitCsf);
    DbgPrintf(0x16, 7, dbgBtlParamRegStaFmt, st->regSta[0]);
    for (j = 1; j < 12; j++) {
        DbgPrintf(0x1E, j + 7, dbgBtlFloatFmt, st->regSta[j]);
    }
}
