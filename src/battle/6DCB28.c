#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"
#include "battle/n34.h"
#include "protos/battle.h"

extern char dbgBtlParamIdFmt[];
extern char dbgBtlParamIconFmt[];
extern char dbgBtlParamSizeFmt[];
extern char dbgBtlParamRaceFmt[];
extern char dbgBtlParamTypeFmt[];
extern char dbgBtlParamBitStaFmt[];
extern char dbgBtlParamBitSta2Fmt[];
extern char dbgBtlParamAtkFmt[];
extern char dbgBtlParamDefFmt[];
extern char dbgBtlParamMgpFmt[];
extern char dbgBtlParamMgrFmt[];
extern char dbgBtlParamActFmt[];
extern char dbgBtlParamMovFmt[];
extern char dbgBtlParamIpStopDmgFmt[];
extern char dbgBtlParamIpStopDownFmt[];
extern char dbgBtlParamLvFmt[];
extern char dbgBtlParamExpFmt[];
extern char dbgBtlParamNextExpFmt[];
extern char dbgBtlParamMhpFmt[];
extern char dbgBtlParamHpFmt[];
extern char dbgBtlParamMmpFmt[];
extern char dbgBtlParamMpFmt[];
extern char dbgBtlParamMspFmt[];
extern char dbgBtlParamSpFmt[];
extern char dbgBtlParamStrFmt[];
extern char dbgBtlParamStaFmt[];
extern char dbgBtlParamItlFmt[];
extern char dbgBtlParamSpiFmt[];
extern char dbgBtlParamAgiFmt[];
extern char dbgBtlParamRunFmt[];
extern char dbgBtlParamComboMaxFmt[];

void DbgBtlCharParamBasePage(s32 idx) {
    PtyUnit *d = ((BtlUnit *)((idx << 9) + (u32)btlUnits))->params; /* integer index: the shift is scheduled ahead of the base address */

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlParamIdFmt, d->id);
    DbgPrintf(2, 5, dbgBtlParamIconFmt, d->icon);
    DbgPrintf(2, 6, dbgBtlParamSizeFmt, d->size);
    DbgPrintf(2, 7, dbgBtlParamRaceFmt, d->race[0]);
    DbgPrintf(11, 8, dbgBtlIntFmt, d->race[1]);
    DbgPrintf(2, 9, dbgBtlParamTypeFmt, d->type);
    DbgPrintf(2, 10, dbgBtlParamBitStaFmt, d->status);
    DbgPrintf(2, 11, dbgBtlParamBitSta2Fmt, d->status2);
    DbgPrintf(2, 12, dbgBtlParamAtkFmt, d->params[0]);
    DbgPrintf(2, 13, dbgBtlParamDefFmt, d->params[1]);
    DbgPrintf(2, 14, dbgBtlParamMgpFmt, d->params[2]);
    DbgPrintf(2, 15, dbgBtlParamMgrFmt, d->params[3]);
    DbgPrintf(2, 16, dbgBtlParamActFmt, d->params[4]);
    DbgPrintf(2, 17, dbgBtlParamMovFmt, d->params[5]);
    DbgPrintf(2, 18, dbgBtlParamIpStopDmgFmt, d->ipStopDmg);
    DbgPrintf(2, 19, dbgBtlParamIpStopDownFmt, d->ipStopDown);
    DbgPrintf(22, 4, dbgBtlParamLvFmt, d->level);
    DbgPrintf(22, 5, dbgBtlParamExpFmt, d->exp);
    DbgPrintf(22, 6, dbgBtlParamNextExpFmt, d->nextExp);
    DbgPrintf(22, 7, dbgBtlParamMhpFmt, d->maxHp);
    DbgPrintf(22, 8, dbgBtlParamHpFmt, d->hp);
    DbgPrintf(22, 9, dbgBtlParamMmpFmt, d->maxMp);
    DbgPrintf(22, 10, dbgBtlParamMpFmt, d->mp);
    DbgPrintf(22, 11, dbgBtlParamMspFmt, d->maxSp);
    DbgPrintf(22, 12, dbgBtlParamSpFmt, d->sp);
    DbgPrintf(22, 13, dbgBtlParamStrFmt, d->stats[0]);
    DbgPrintf(22, 14, dbgBtlParamStaFmt, d->stats[1]);
    DbgPrintf(22, 15, dbgBtlParamItlFmt, d->stats[2]);
    DbgPrintf(22, 16, dbgBtlParamSpiFmt, d->stats[3]);
    DbgPrintf(22, 17, dbgBtlParamAgiFmt, d->stats[4]);
    DbgPrintf(22, 18, dbgBtlParamRunFmt, d->stats[5]);
    DbgPrintf(22, 19, dbgBtlParamComboMaxFmt, d->comboMax);
}
