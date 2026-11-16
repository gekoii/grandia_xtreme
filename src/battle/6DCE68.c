#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"
#include "battle/n34.h"
#include "protos/battle.h"

extern char dbgBtlParamGuardDecFmt[];
extern char dbgBtlParamAvoidWaitFmt[];
extern char dbgBtlParamAvoidEvadeFmt[];
extern char dbgBtlParamItemUpBombFmt[];
extern char dbgBtlParamItemUpScrFmt[];
extern char dbgBtlParamItemUpDrugFmt[];
extern char dbgBtlParamEqpAttrFmt[];
extern char dbgBtlParamEqpKillerFmt[];
extern char dbgBtlParamEqpBitStaFmt[];
extern char dbgBtlParamEqpStaProbFmt[];
extern char dbgBtlParamAbsorbHpFmt[];
extern char dbgBtlParamCrtclUpFmt[];
extern char dbgBtlParamComboUpFmt[];
extern char dbgBtlParamCrtclMultiFmt[];
extern char dbgBtlParamAtkDownFmt[];
extern char dbgBtlParamDefDownFmt[];
extern char dbgBtlParamKillerFmt[];

void DbgBtlCharParamEquipPage(s32 idx) {
    PtyUnit *st = ((BtlUnit *)((idx << 9) + (u32)btlUnits))->params; /* integer index: the shift is scheduled ahead of the base address */
    s32 i;

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlParamGuardDecFmt, st->guardDec);
    DbgPrintf(2, 5, dbgBtlParamAvoidWaitFmt, st->avoidWait);
    DbgPrintf(2, 6, dbgBtlParamAvoidEvadeFmt, st->avoidEvade);
    DbgPrintf(2, 7, dbgBtlParamItemUpBombFmt, st->itemUpBomb);
    DbgPrintf(2, 8, dbgBtlParamItemUpScrFmt, st->itemUpScr);
    DbgPrintf(2, 9, dbgBtlParamItemUpDrugFmt, st->itemUpDrug);
    DbgPrintf(2, 10, dbgBtlParamEqpAttrFmt, st->eqpAttr);
    DbgPrintf(2, 11, dbgBtlParamEqpKillerFmt, st->eqpKiller);
    DbgPrintf(2, 12, dbgBtlParamEqpBitStaFmt, st->eqpBitSta);
    DbgPrintf(2, 13, dbgBtlParamEqpStaProbFmt, st->eqpStaProb);
    DbgPrintf(2, 14, dbgBtlParamAbsorbHpFmt, st->absorbHp);
    DbgPrintf(2, 15, dbgBtlParamCrtclUpFmt, st->crtclUp);
    DbgPrintf(2, 16, dbgBtlParamComboUpFmt, st->comboUp);
    DbgPrintf(2, 17, dbgBtlParamCrtclMultiFmt, st->crtclMulti);
    DbgPrintf(2, 18, dbgBtlParamAtkDownFmt, st->atkDown);
    DbgPrintf(2, 19, dbgBtlParamDefDownFmt, st->defDown);
    DbgPrintf(0x16, 4, dbgBtlParamKillerFmt, st->killer[0]);
    for (i = 1; i < 13; i++) {
        DbgPrintf(0x1D, i + 4, dbgBtlFloatFmt, st->killer[i]);
    }
}
