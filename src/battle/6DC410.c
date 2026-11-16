#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "pad.h"
#include "sys.h"
#include "battle/n34.h"


extern char dbgBtlWorkComboFmt[];
extern char dbgBtlWorkAtkNoFmt[];
extern char dbgBtlWorkScpTypeFmt[];
extern char dbgBtlWorkScpRFmt[];
extern char dbgBtlWorkMgcNoFmt[];
extern char dbgBtlWorkMgcLvFmt[];
extern char dbgBtlWorkPartsChrFmt[];
extern char dbgBtlWorkRtActFmt[];
extern char dbgBtlWorkRtPFmt[];
extern char dbgBtlWorkBodyFmt[];
extern char dbgBtlWorkItemFmt[];
extern char dbgBtlWorkItemRateFmt[];
extern char dbgBtlWorkTreasureFmt[];
extern char dbgBtlWorkMvRotVFmt[];
extern char dbgBtlWorkWtRotVFmt[];
extern char dbgBtlWorkRotVMaxFmt[];
extern char dbgBtlWorkEggNoFmt[];
extern char dbgBtlWorkEggMgcFmt[];

void DbgBtlCharWorkAttackPage(s32 idx) {
    BtlUnit *u = &btlUnits[idx];
    s32 i;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 0);
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlWorkComboFmt, u->combo);
    DbgPrintf(2, 5, dbgBtlWorkAtkNoFmt, u->action);
    DbgPrintf(2, 6, dbgBtlWorkScpTypeFmt, u->scopeType);
    DbgPrintf(7, 7, dbgBtlWorkScpRFmt, u->scopeRadius);
    DbgPrintf(2, 8, dbgBtlWorkMgcNoFmt, u->skillId);
    DbgPrintf(6, 9, dbgBtlWorkMgcLvFmt, u->skillLevel);
    DbgPrintf(2, 10, dbgBtlWorkPartsChrFmt, u->partsChr);
    DbgPrintf(5, 11, dbgBtlWorkRtActFmt, u->rtAct);
    DbgPrintf(8, 12, dbgBtlWorkRtPFmt, u->rtP);
    DbgPrintf(7, 13, dbgBtlWorkBodyFmt, u->body);
    for (i = 0; i < 3; i++) {
        DbgPrintf(10, i + 14, dbgBtlIndexValueFmt, i, u->parts[i]);
    }
    DbgPrintf(2, 17, dbgBtlWorkItemFmt, u->item);
    DbgPrintf(4, 18, dbgBtlWorkItemRateFmt, u->itemRate);
    DbgPrintf(2, 19, dbgBtlWorkTreasureFmt, u->treasure);
    DbgPrintf(0x16, 4, dbgBtlWorkMvRotVFmt, u->rotVel);
    DbgPrintf(0x16, 5, dbgBtlWorkWtRotVFmt, u->rotAccel);
    DbgPrintf(0x16, 6, dbgBtlWorkRotVMaxFmt, u->rotVelMax);
    DbgPrintf(0x16, 7, dbgBtlWorkEggNoFmt, u->eggNo[0]);
    DbgPrintf(0x1A, 13, dbgBtlWorkEggMgcFmt, u->eggMagic[0]);
    for (i = 1; i < 6; i++) {
        DbgPrintf(0x1E, i + 7, dbgBtlIntFmt, u->eggNo[i]);
        DbgPrintf(0x1E, i + 13, dbgBtlIntFmt, u->eggMagic[i]);
    }
}
