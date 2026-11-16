#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"
#include "protos/battle.h"

extern char dbgBtlParamGetGoldRateFmt[];
extern char dbgBtlParamGetRateUpFmt[];
extern char dbgBtlParamGetItemUpFmt[];
extern char dbgBtlParamGetExpRateFmt[];
extern char dbgBtlParamCombiHintFmt[];
extern char dbgBtlParamMcActDispFmt[];
extern char dbgBtlParamLvupMhpFmt[];
extern char dbgBtlParamLvupStrFmt[];
extern char dbgBtlParamLvupStaFmt[];
extern char dbgBtlParamLvupItlFmt[];
extern char dbgBtlParamLvupSpiFmt[];
extern char dbgBtlParamLvupAgiFmt[];
extern char dbgBtlParamLvupRunFmt[];
extern char dbgBtlParamStartIpOfsFmt[];
extern char dbgBtlParamMcAwayDecFmt[];
extern char dbgBtlParamAtkRefFmt[];
extern char dbgBtlParamStartTranspFmt[];
extern char dbgBtlParamStartSleepFmt[];
extern char dbgBtlParamStartParaFmt[];
extern char dbgBtlParamExeSpFmt[];
extern char dbgBtlParamExeGoodFmt[];
extern char dbgBtlParamExeBadFmt[];
extern char dbgBtlParamSpSupCmbFmt[];
extern char dbgBtlParamSpSupCrtFmt[];
extern char dbgBtlParamSpSupDmgFmt[];
extern char dbgBtlParamSpSupDownFmt[];
extern char dbgBtlParamSpSupGuardFmt[];
extern char dbgBtlParamRestartItemFmt[];
extern char dbgBtlParamRestartEquipFmt[];
extern char dbgBtlParamRestartGuardFmt[];
extern char dbgBtlParamRestartEvadeFmt[];

void DbgBtlCharParamGrowthPage(s32 idx) {
    PtyUnit *d = ((BtlUnit *)((idx << 9) + (u32)btlUnits))->params; /* integer index: the shift is scheduled ahead of the base address */

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlParamGetGoldRateFmt, d->getGoldRate);
    DbgPrintf(2, 5, dbgBtlParamGetRateUpFmt, d->getRateUp);
    DbgPrintf(2, 6, dbgBtlParamGetItemUpFmt, d->getItemUp);
    DbgPrintf(2, 7, dbgBtlParamGetExpRateFmt, d->getExpRate);
    DbgPrintf(2, 8, dbgBtlParamCombiHintFmt, d->combiHint);
    DbgPrintf(2, 9, dbgBtlParamMcActDispFmt, d->mcActDisp);
    DbgPrintf(2, 10, dbgBtlParamLvupMhpFmt, d->lvupMhp);
    DbgPrintf(2, 11, dbgBtlParamLvupStrFmt, d->lvupStats[0]);
    DbgPrintf(2, 12, dbgBtlParamLvupStaFmt, d->lvupStats[1]);
    DbgPrintf(2, 13, dbgBtlParamLvupItlFmt, d->lvupStats[2]);
    DbgPrintf(2, 14, dbgBtlParamLvupSpiFmt, d->lvupStats[3]);
    DbgPrintf(2, 15, dbgBtlParamLvupAgiFmt, d->lvupStats[4]);
    DbgPrintf(2, 16, dbgBtlParamLvupRunFmt, d->lvupStats[5]);
    DbgPrintf(2, 17, dbgBtlParamStartIpOfsFmt, d->startIpOfs);
    DbgPrintf(2, 18, dbgBtlParamMcAwayDecFmt, d->mcAwayDec);
    DbgPrintf(22, 4, dbgBtlParamAtkRefFmt, d->atkRef);
    DbgPrintf(22, 5, dbgBtlParamStartTranspFmt, d->startTransp);
    DbgPrintf(22, 6, dbgBtlParamStartSleepFmt, d->startSleep);
    DbgPrintf(22, 7, dbgBtlParamStartParaFmt, d->startPara);
    DbgPrintf(22, 8, dbgBtlParamExeSpFmt, d->exeSp);
    DbgPrintf(22, 9, dbgBtlParamExeGoodFmt, d->exeGood);
    DbgPrintf(22, 10, dbgBtlParamExeBadFmt, d->exeBad);
    DbgPrintf(22, 11, dbgBtlParamSpSupCmbFmt, d->spSupCmb);
    DbgPrintf(22, 12, dbgBtlParamSpSupCrtFmt, d->spSupCrt);
    DbgPrintf(22, 13, dbgBtlParamSpSupDmgFmt, d->spSupDmg);
    DbgPrintf(22, 14, dbgBtlParamSpSupDownFmt, d->spSupDown);
    DbgPrintf(22, 15, dbgBtlParamSpSupGuardFmt, d->spSupGuard);
    DbgPrintf(22, 16, dbgBtlParamRestartItemFmt, d->restartItem);
    DbgPrintf(22, 17, dbgBtlParamRestartEquipFmt, d->restartEquip);
    DbgPrintf(22, 18, dbgBtlParamRestartGuardFmt, d->restartGuard);
    DbgPrintf(22, 19, dbgBtlParamRestartEvadeFmt, d->restartEvade);
}
