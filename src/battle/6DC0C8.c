#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "pad.h"
#include "sys.h"


extern char dbgBtlWorkExeSpFmt[];
extern char dbgBtlWorkGoodFmt[];
extern char dbgBtlWorkBadFmt[];
extern char dbgBtlWorkTranspFmt[];
extern char dbgBtlWorkAntiMFmt[];
extern char dbgBtlWorkRegeneFmt[];
extern char dbgBtlWorkSleepFmt[];
extern char dbgBtlWorkConfuFmt[];
extern char dbgBtlWorkSealSFmt[];
extern char dbgBtlWorkSealMFmt[];
extern char dbgBtlWorkBindFmt[];
extern char dbgBtlWorkVanishFmt[];
extern char dbgBtlWorkExpFmt[];
extern char dbgBtlWorkWallFmt[];
extern char dbgBtlWorkPowUpFmt[];
extern char dbgBtlWorkAtkFmt[];
extern char dbgBtlWorkDefFmt[];
extern char dbgBtlWorkMgrFmt[];
extern char dbgBtlWorkActFmt[];
extern char dbgBtlWorkMovFmt[];
extern char dbgBtlWorkIpModeFmt[];
extern char dbgBtlWorkIpValFmt[];
extern char dbgBtlWorkIpSpdFmt[];
extern char dbgBtlWorkIpDmgFmt[];
extern char dbgBtlWorkIpRateFmt[];
extern char dbgBtlWorkIpStopFmt[];
extern char dbgBtlWorkIpRetFmt[];
extern char dbgBtlWorkMvpFmt[];

void DbgBtlCharWorkStatusPage(s32 idx) {
    BtlUnit *d = &btlUnits[idx];

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 0);
    }

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlWorkExeSpFmt, d->exeSp);
    DbgPrintf(4, 5, dbgBtlWorkGoodFmt, d->exeGood);
    DbgPrintf(5, 6, dbgBtlWorkBadFmt, d->exeBad);
    DbgPrintf(2, 7, dbgBtlWorkTranspFmt, d->transp);
    DbgPrintf(2, 8, dbgBtlWorkAntiMFmt, d->antiMagic);
    DbgPrintf(2, 9, dbgBtlWorkRegeneFmt, d->regene);
    DbgPrintf(2, 10, dbgBtlWorkSleepFmt, d->sleep);
    DbgPrintf(2, 11, dbgBtlWorkConfuFmt, d->confu);
    DbgPrintf(2, 12, dbgBtlWorkSealSFmt, d->sealS);
    DbgPrintf(2, 13, dbgBtlWorkSealMFmt, d->sealM);
    DbgPrintf(2, 14, dbgBtlWorkBindFmt, d->bind);
    DbgPrintf(2, 15, dbgBtlWorkVanishFmt, d->vanish);
    DbgPrintf(2, 16, dbgBtlWorkExpFmt, d->exp);
    DbgPrintf(2, 17, dbgBtlWorkWallFmt, d->wall);
    DbgPrintf(22, 4, dbgBtlWorkPowUpFmt, d->powUp);
    DbgPrintf(22, 5, dbgBtlWorkAtkFmt, d->statMod[0]);
    DbgPrintf(22, 6, dbgBtlWorkDefFmt, d->statMod[1]);
    DbgPrintf(22, 7, dbgBtlWorkMgrFmt, d->statMod[2]);
    DbgPrintf(22, 8, dbgBtlWorkActFmt, d->statMod[3]);
    DbgPrintf(22, 9, dbgBtlWorkMovFmt, d->statMod[4]);
    DbgPrintf(22, 10, dbgBtlWorkIpModeFmt, d->ipMode);
    DbgPrintf(24, 11, dbgBtlWorkIpValFmt, d->ipPos);
    DbgPrintf(24, 12, dbgBtlWorkIpSpdFmt, d->ipSpeed);
    DbgPrintf(24, 13, dbgBtlWorkIpDmgFmt, d->ipDamage);
    DbgPrintf(23, 14, dbgBtlWorkIpRateFmt, d->ipRate);
    DbgPrintf(23, 15, dbgBtlWorkIpStopFmt, d->ipStop);
    DbgPrintf(24, 16, dbgBtlWorkIpRetFmt, d->ipRet);
    DbgPrintf(22, 17, dbgBtlWorkMvpFmt, d->mvp);
}
