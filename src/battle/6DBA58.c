#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "pad.h"
#include "sys.h"


extern char dbgBtlWorkPosXFmt[];
extern char dbgBtlWorkPosYFmt[];
extern char dbgBtlWorkPosZFmt[];
extern char dbgBtlWorkAngXFmt[];
extern char dbgBtlWorkAngYFmt[];
extern char dbgBtlWorkAngZFmt[];
extern char dbgBtlWorkSclXFmt[];
extern char dbgBtlWorkSclYFmt[];
extern char dbgBtlWorkSclZFmt[];
extern char dbgBtlWorkAlphaFmt[];
extern char dbgBtlWorkAlphaDFmt[];
extern char dbgBtlWorkAmbRFmt[];
extern char dbgBtlWorkAmbGFmt[];
extern char dbgBtlWorkAmbBFmt[];
extern char dbgBtlWorkCountFmt[];
extern char dbgBtlWorkActIdFmt[];
extern char dbgBtlWorkPhaseFmt[];
extern char dbgBtlWorkSubFmt[];
extern char dbgBtlWorkRetIdFmt[];
extern char dbgBtlWorkRetPFmt[];
extern char dbgBtlWorkTargetFmt[];
extern char dbgBtlWorkAtkedFmt[];
extern char dbgBtlWorkStateFmt[];
extern char dbgBtlWorkKbVFmt[];
extern char dbgBtlWorkRunFmt[];
extern char dbgBtlWorkAiFmt[];
extern char dbgBtlWorkTurnFmt[];
extern char dbgBtlWorkFlagFmt[];
extern char dbgBtlWorkDustFmt[];

void DbgBtlCharWorkTransformPage(s32 idx) {
    BtlUnit *d = &btlUnits[idx];

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 14);
        switch (sysState.dbgCursor) {
        case 1: d->pos[0] += DbgEditValue(2, -1, 3, 10.0f); break;
        case 2: d->pos[1] += DbgEditValue(2, -1, 3, 10.0f); break;
        case 3: d->pos[2] += DbgEditValue(2, -1, 3, 10.0f); break;
        case 4: d->rot[0] += DbgEditValue(1, 0, 5, 256.0f); break;
        case 5: d->rot[1] += DbgEditValue(1, 0, 5, 256.0f); break;
        case 6: d->rot[2] += DbgEditValue(1, 0, 5, 256.0f); break;
        case 7: d->scale[0] += DbgEditValue(2, -2, 1, 0.1f); break;
        case 8: d->scale[1] += DbgEditValue(2, -2, 1, 0.1f); break;
        case 9: d->scale[2] += DbgEditValue(2, -2, 1, 0.1f); break;
        case 10: d->alpha += DbgEditValue(2, -3, 1, 0.1f); break;
        case 11: d->alphaStep += DbgEditValue(2, -3, 1, 0.1f); break;
        case 12: d->ambient[0] += DbgEditValue(2, 0, 3, 10.0f); break;
        case 13: d->ambient[1] += DbgEditValue(2, 0, 3, 10.0f); break;
        case 14: d->ambient[2] += DbgEditValue(2, 0, 3, 10.0f); break;
        }
    }

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 4, dbgBtlWorkPosXFmt, d->pos[0]);
    DbgPrintf(6, 5, dbgBtlWorkPosYFmt, d->pos[1]);
    DbgPrintf(6, 6, dbgBtlWorkPosZFmt, d->pos[2]);
    DbgPrintf(2, 7, dbgBtlWorkAngXFmt, d->rot[0] & 0xFFFFF);
    DbgPrintf(6, 8, dbgBtlWorkAngYFmt, d->rot[1] & 0xFFFFF);
    DbgPrintf(6, 9, dbgBtlWorkAngZFmt, d->rot[2] & 0xFFFFF);
    DbgPrintf(2, 10, dbgBtlWorkSclXFmt, d->scale[0]);
    DbgPrintf(6, 11, dbgBtlWorkSclYFmt, d->scale[1]);
    DbgPrintf(6, 12, dbgBtlWorkSclZFmt, d->scale[2]);
    DbgPrintf(2, 13, dbgBtlWorkAlphaFmt, d->alpha);
    DbgPrintf(6, 14, dbgBtlWorkAlphaDFmt, d->alphaStep);
    DbgPrintf(2, 15, dbgBtlWorkAmbRFmt, d->ambient[0]);
    DbgPrintf(6, 16, dbgBtlWorkAmbGFmt, d->ambient[1]);
    DbgPrintf(6, 17, dbgBtlWorkAmbBFmt, d->ambient[2]);
    DbgPrintf(2, 18, dbgBtlWorkCountFmt, d->stateCount);
    DbgPrintf(22, 4, dbgBtlWorkActIdFmt, d->state);
    DbgPrintf(22, 5, dbgBtlWorkPhaseFmt, d->phase);
    DbgPrintf(25, 6, dbgBtlWorkSubFmt, d->subPhase);
    DbgPrintf(22, 7, dbgBtlWorkRetIdFmt, d->retState);
    DbgPrintf(27, 8, dbgBtlWorkRetPFmt, d->retPhase);
    DbgPrintf(22, 9, dbgBtlWorkTargetFmt, d->target);
    DbgPrintf(22, 10, dbgBtlWorkAtkedFmt, d->attacker);
    DbgPrintf(22, 11, dbgBtlWorkStateFmt, d->flags);
    DbgPrintf(22, 12, dbgBtlWorkKbVFmt, d->knockback);
    DbgPrintf(22, 13, dbgBtlWorkRunFmt, d->run);
    DbgPrintf(22, 14, dbgBtlWorkAiFmt, d->ai);
    DbgPrintf(22, 15, dbgBtlWorkTurnFmt, d->turn);
    DbgPrintf(22, 16, dbgBtlWorkFlagFmt, d->flag);
    DbgPrintf(22, 17, dbgBtlWorkDustFmt, d->dust);
}
