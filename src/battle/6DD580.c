#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"
#include "protos/battle.h"

extern char dbgBtlParamSpcSpdRateFmt[];
extern char dbgBtlParamSpcRetRateFmt[];
extern char dbgBtlParamSpcPowFmt[];
extern char dbgBtlParamSpcSpRateFmt[];
extern char dbgBtlParamMgcSpdRateFmt[];
extern char dbgBtlParamMgcRetRateFmt[];
extern char dbgBtlParamMgcMpRateFmt[];
extern char dbgBtlParamMgc1IpCanFmt[];
extern char dbgBtlParamMultiMagicFmt[];
extern char dbgBtlParamItmSpdRateFmt[];
extern char dbgBtlParamGuardHpFmt[];
extern char dbgBtlParamGuardSpFmt[];
extern char dbgBtlParamEscapeUpFmt[];
extern char dbgBtlParamStrikeBackFmt[];
extern char dbgBtlParamCmdDisableFmt[];
extern char dbgBtlParamNoDamageFmt[];
extern char dbgBtlParamRefDamageFmt[];
extern char dbgBtlParamTakeDmgMpFmt[];
extern char dbgBtlParamAutoGuardFmt[];
extern char dbgBtlParamAutoYomiFmt[];
extern char dbgBtlParamAngryRateFmt[];
extern char dbgBtlParamAngryProbFmt[];
extern char dbgBtlParamAngryFmt[];
extern char dbgBtlParamSArmorRateFmt[];
extern char dbgBtlParamLureFmt[];
extern char dbgBtlParamAntiLvDrainFmt[];
extern char dbgBtlParamAntiKbFmt[];
extern char dbgBtlParamAntiRetireFmt[];
extern char dbgBtlParamAntiCounterFmt[];
extern char dbgBtlParamAntiCancelFmt[];
extern char dbgBtlParamAntiHalfFmt[];

void DbgBtlCharParamAbilityPage(s32 idx) {
    PtyUnit *d = ((BtlUnit *)((idx << 9) + (u32)btlUnits))->params; /* integer index: the shift is scheduled ahead of the base address */

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlParamSpcSpdRateFmt, d->spcSpdRate);
    DbgPrintf(2, 5, dbgBtlParamSpcRetRateFmt, d->spcRetRate);
    DbgPrintf(2, 6, dbgBtlParamSpcPowFmt, d->spcPow);
    DbgPrintf(2, 7, dbgBtlParamSpcSpRateFmt, d->spcSpRate);
    DbgPrintf(2, 8, dbgBtlParamMgcSpdRateFmt, d->mgcSpdRate);
    DbgPrintf(2, 9, dbgBtlParamMgcRetRateFmt, d->mgcRetRate);
    DbgPrintf(2, 10, dbgBtlParamMgcMpRateFmt, d->mgcMpRate);
    DbgPrintf(2, 11, dbgBtlParamMgc1IpCanFmt, d->mgc1IpCan);
    DbgPrintf(2, 12, dbgBtlParamMultiMagicFmt, d->multiMagic);
    DbgPrintf(2, 13, dbgBtlParamItmSpdRateFmt, d->itmSpdRate);
    DbgPrintf(2, 14, dbgBtlParamGuardHpFmt, d->guardHp);
    DbgPrintf(2, 15, dbgBtlParamGuardSpFmt, d->guardSp);
    DbgPrintf(2, 16, dbgBtlParamEscapeUpFmt, d->escapeUp);
    DbgPrintf(2, 17, dbgBtlParamStrikeBackFmt, d->strikeBack);
    DbgPrintf(2, 18, dbgBtlParamCmdDisableFmt, d->cmdDisable);
    DbgPrintf(22, 4, dbgBtlParamNoDamageFmt, d->noDamage);
    DbgPrintf(22, 5, dbgBtlParamRefDamageFmt, d->refDamage);
    DbgPrintf(22, 6, dbgBtlParamTakeDmgMpFmt, d->takeDmgMp);
    DbgPrintf(22, 7, dbgBtlParamAutoGuardFmt, d->autoGuard);
    DbgPrintf(22, 8, dbgBtlParamAutoYomiFmt, d->autoYomi);
    DbgPrintf(22, 9, dbgBtlParamAngryRateFmt, d->angryRate);
    DbgPrintf(22, 10, dbgBtlParamAngryProbFmt, d->angryProb);
    DbgPrintf(22, 11, dbgBtlParamAngryFmt, d->angry);
    DbgPrintf(22, 12, dbgBtlParamSArmorRateFmt, d->sArmorRate);
    DbgPrintf(22, 13, dbgBtlParamLureFmt, d->lure);
    DbgPrintf(22, 14, dbgBtlParamAntiLvDrainFmt, d->antiLvDrain);
    DbgPrintf(22, 15, dbgBtlParamAntiKbFmt, d->antiKb);
    DbgPrintf(22, 16, dbgBtlParamAntiRetireFmt, d->antiRetire);
    DbgPrintf(22, 17, dbgBtlParamAntiCounterFmt, d->antiCounter);
    DbgPrintf(22, 18, dbgBtlParamAntiCancelFmt, d->antiCancel);
    DbgPrintf(22, 19, dbgBtlParamAntiHalfFmt, d->antiHalf);
}
