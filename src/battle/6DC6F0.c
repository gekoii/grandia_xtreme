#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "pad.h"
#include "sys.h"
#include "battle/n34.h"


extern char dbgBtlWorkMotFmt[];
extern char dbgBtlWorkAnmFmt[];
extern char dbgBtlWorkAnmSpFmt[];
extern char dbgBtlWorkAnmStartFmt[];
extern char dbgBtlWorkAnmEndFmt[];
extern char dbgBtlWorkAnmLoopFmt[];
extern char dbgBtlWorkHeadLookFmt[];
extern char dbgBtlWorkWakeFmt[];
extern char dbgBtlWorkCombiLFmt[];
extern char dbgBtlWorkFaceModeFmt[];
extern char dbgBtlWorkFaceIdFmt[];
extern char dbgBtlWorkFaceC1Fmt[];
extern char dbgBtlWorkFaceC2Fmt[];
extern char dbgBtlWorkFaceC3Fmt[];

void DbgBtlCharWorkMotionPage(s32 idx) {
    BtlUnit *u = &btlUnits[idx];
    s32 i;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 0);
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlWorkMotFmt, u->motionNo);
    DbgPrintf(2, 5, dbgBtlWorkAnmFmt, u->frame);
    DbgPrintf(6, 6, dbgBtlWorkAnmSpFmt, u->frameSpeed);
    DbgPrintf(7, 7, dbgBtlWorkAnmStartFmt, u->loopStart);
    DbgPrintf(7, 8, dbgBtlWorkAnmEndFmt, u->loopEnd);
    DbgPrintf(4, 9, dbgBtlWorkAnmLoopFmt, u->loopCount);
    DbgPrintf(2, 10, dbgBtlWorkHeadLookFmt, (s8)u->headLook);
    DbgPrintf(2, 11, dbgBtlWorkWakeFmt, u->wake);
    DbgPrintf(3, 12, dbgBtlWorkCombiLFmt, u->comboLeader);
    for (i = 0; i < 3; i++) {
        DbgPrintf(8, i + 13, dbgBtlIndexValueFmt, i, u->comboPartners[i]);
    }
    DbgPrintf(0x16, 4, dbgBtlWorkFaceModeFmt, u->face.mode);
    DbgPrintf(0x1B, 5, dbgBtlWorkFaceIdFmt, u->face.face);
    DbgPrintf(0x1B, 6, dbgBtlWorkFaceC1Fmt, u->face.blinkStep);
    DbgPrintf(0x1B, 7, dbgBtlWorkFaceC2Fmt, u->face.blinkTimer);
    DbgPrintf(0x1B, 8, dbgBtlWorkFaceC3Fmt, u->face.stepTimer);
}
