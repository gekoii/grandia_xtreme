#include "common.h"
#include "sys.h"
#include "party.h"
#include "protos/battle.h"

extern char dbgBtlPcSaveExpFmt[];
extern char dbgBtlPcSaveMhpFmt[];
extern char dbgBtlPcSaveHpFmt[];
extern char dbgBtlPcSaveMspFmt[];
extern char dbgBtlPcSaveSpFmt[];
extern char dbgBtlPcSaveStrFmt[];
extern char dbgBtlPcSaveStaFmt[];
extern char dbgBtlPcSaveItlFmt[];
extern char dbgBtlPcSaveSpiFmt[];
extern char dbgBtlPcSaveAgiFmt[];
extern char dbgBtlPcSaveRunFmt[];
extern char dbgBtlPcSaveBitStaFmt[];
extern char dbgBtlPcSaveAiFmt[];
extern char dbgBtlPcSaveAi2Fmt[];
extern char dbgBtlPcSaveMhpUpFmt[];
extern char dbgBtlPcSaveMspUpFmt[];
extern char dbgBtlPcSaveStrUpFmt[];
extern char dbgBtlPcSaveStaUpFmt[];
extern char dbgBtlPcSaveItlUpFmt[];
extern char dbgBtlPcSaveSpiUpFmt[];
extern char dbgBtlPcSaveAgiUpFmt[];
extern char dbgBtlPcSaveRunUpFmt[];
extern char dbgBtlPcSaveBtlCountFmt[];
extern char dbgBtlPcSaveKillFmt[];
extern char dbgBtlPcSaveKillMaxFmt[];
extern char dbgBtlPcSaveKillTotalFmt[];
extern char dbgBtlPcSaveDeadFmt[];
extern char dbgBtlPcSaveMvpFmt[];

void DbgBtlPcSaveStatsPage(s32 idx) {
    PtyChar *d = &ptyCharData[idx];

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlPcSaveExpFmt, d->exp);
    DbgPrintf(2, 5, dbgBtlPcSaveMhpFmt, d->maxHp);
    DbgPrintf(2, 6, dbgBtlPcSaveHpFmt, d->hp);
    DbgPrintf(2, 7, dbgBtlPcSaveMspFmt, d->maxSp);
    DbgPrintf(2, 8, dbgBtlPcSaveSpFmt, d->sp);
    DbgPrintf(2, 9, dbgBtlPcSaveStrFmt, d->stats[0]);
    DbgPrintf(2, 10, dbgBtlPcSaveStaFmt, d->stats[1]);
    DbgPrintf(2, 11, dbgBtlPcSaveItlFmt, d->stats[2]);
    DbgPrintf(2, 12, dbgBtlPcSaveSpiFmt, d->stats[3]);
    DbgPrintf(2, 13, dbgBtlPcSaveAgiFmt, d->stats[4]);
    DbgPrintf(2, 14, dbgBtlPcSaveRunFmt, d->stats[5]);
    DbgPrintf(2, 15, dbgBtlPcSaveBitStaFmt, d->status);
    DbgPrintf(2, 16, dbgBtlPcSaveAiFmt, d->tactics[0]);
    DbgPrintf(2, 17, dbgBtlPcSaveAi2Fmt, d->tactics[1]);
    DbgPrintf(22, 4, dbgBtlPcSaveMhpUpFmt, d->hpGrowth);
    DbgPrintf(22, 5, dbgBtlPcSaveMspUpFmt, d->spGrowth);
    DbgPrintf(22, 6, dbgBtlPcSaveStrUpFmt, d->statGrowth[0]);
    DbgPrintf(22, 7, dbgBtlPcSaveStaUpFmt, d->statGrowth[1]);
    DbgPrintf(22, 8, dbgBtlPcSaveItlUpFmt, d->statGrowth[2]);
    DbgPrintf(22, 9, dbgBtlPcSaveSpiUpFmt, d->statGrowth[3]);
    DbgPrintf(22, 10, dbgBtlPcSaveAgiUpFmt, d->statGrowth[4]);
    DbgPrintf(22, 11, dbgBtlPcSaveRunUpFmt, d->statGrowth[5]);
    DbgPrintf(22, 12, dbgBtlPcSaveBtlCountFmt, d->battleCount);
    DbgPrintf(22, 13, dbgBtlPcSaveKillFmt, d->kills);
    DbgPrintf(22, 14, dbgBtlPcSaveKillMaxFmt, d->killMax);
    DbgPrintf(22, 15, dbgBtlPcSaveKillTotalFmt, d->killTotal);
    DbgPrintf(22, 16, dbgBtlPcSaveDeadFmt, d->deaths);
    DbgPrintf(22, 17, dbgBtlPcSaveMvpFmt, d->mvp);
}
