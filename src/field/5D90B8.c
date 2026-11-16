#include "common.h"
#include "pad.h"
#include "save.h"
#include "protos/field.h"
#include "field/n16.h"

extern s32 dbgFldMainFlagLine;
extern u8 dbgMainFlagStrMapNo[];
extern u8 dbgMainFlagStrOldMapNo[];
extern u8 dbgMainFlagStrPartyId[];
extern u8 dbgMainFlagStrRetryCounter[];
extern u8 dbgMainFlagStrAreaMapNo[];
extern u8 dbgMainFlagStrMainFlag[];
extern u8 dbgMainFlagStrEncount[];
extern u8 dbgMainFlagStrTenantMove[];
extern u8 dbgMainFlagStrMonsterMove[];
extern u8 dbgMainFlagStrTenantDisp[];
extern u8 dbgMainFlagStrMonsterDisp[];
extern u8 dbgMainFlagStrDemoStatus[];
extern u8 dbgMainFlagStrDemoTempDisp[];
extern u8 dbgMainFlagStrOcc[];
extern u8 dbgMainFlagStrMealEnd[];

void DbgFldMainFlagPage(void) {
    Pad *pad = &padPort1;
    SysSaveData *d;
    s32 x;

    if (dbgFldMainFlagLine < 15) {
        DbgDrawCursor(3, dbgFldMainFlagLine + 5);
    } else {
        DbgDrawCursor(0x1C, dbgFldMainFlagLine - 7);
    }
    if (!(pad->held & 2)) {
        if ((pad->repeatFast & 0x1000) && dbgFldMainFlagLine != 0) {
            dbgFldMainFlagLine--;
        }
        if ((pad->repeatFast & 0x4000) && dbgFldMainFlagLine < 16) {
            dbgFldMainFlagLine++;
        }
        if (pad->repeatFast & 0x8000) {
            switch (dbgFldMainFlagLine) {
            case 0:
                sysSaveData.mapNo--;
                break;
            case 1:
                sysSaveData.oldMapNo--;
                break;
            case 2:
                break;
            case 3:
                sysSaveData.retryCount--;
                break;
            case 4:
                sysSaveData.areaMapNo--;
                break;
            case 5:
                x = sysSaveData.mainFlag;
                if (pad->held & 1) {
                    x -= 100;
                } else {
                    x -= 1;
                }
                if (x < 0) {
                    x = 0x2E9F;
                }
                sysSaveData.mainFlag = x;
                break;
            case 6:
                EvtSystemFlagSet(0x64, 0);
                break;
            case 7:
                EvtSystemFlagSet(0x70, 0);
                break;
            case 8:
                EvtSystemFlagSet(0x71, 0);
                break;
            case 9:
                EvtSystemFlagSet(0x72, 0);
                break;
            case 10:
                EvtSystemFlagSet(0x73, 0);
                break;
            case 11:
                EvtSystemFlagSet(0x82, 0);
                break;
            case 12:
                EvtSystemFlagSet(0x83, 0);
                break;
            case 13:
                EvtSystemFlagSet(0x84, 0);
                break;
            case 14:
                EvtSystemFlagSet(0xFD, 0);
                break;
            case 15:
                EvtSystemFlagSet(0x6F, 0);
                break;
            case 16:
                EvtSystemFlagSet(0x60, 0);
                break;
            }
        }
        if (pad->repeatFast & 0x2000) {
            switch (dbgFldMainFlagLine) {
            case 0:
                sysSaveData.mapNo++;
                break;
            case 1:
                sysSaveData.oldMapNo++;
                break;
            case 2:
                break;
            case 3:
                sysSaveData.retryCount++;
                break;
            case 4:
                sysSaveData.areaMapNo++;
                break;
            case 5:
                if (pad->held & 1) {
                    sysSaveData.mainFlag += 100;
                } else {
                    sysSaveData.mainFlag += 1;
                }
                break;
            case 6:
                EvtSystemFlagSet(0x64, 1);
                break;
            case 7:
                EvtSystemFlagSet(0x70, 1);
                break;
            case 8:
                EvtSystemFlagSet(0x71, 1);
                break;
            case 9:
                EvtSystemFlagSet(0x72, 1);
                break;
            case 10:
                EvtSystemFlagSet(0x73, 1);
                break;
            case 11:
                EvtSystemFlagSet(0x82, 1);
                break;
            case 12:
                EvtSystemFlagSet(0x83, 1);
                break;
            case 13:
                EvtSystemFlagSet(0x84, 1);
                break;
            case 14:
                EvtSystemFlagSet(0xFD, 1);
                break;
            case 15:
                EvtSystemFlagSet(0x6F, 1);
                break;
            case 16:
                EvtSystemFlagSet(0x60, 1);
                break;
            }
        }
    }
    d = &sysSaveData;
    DbgPrintf(4, 5, dbgMainFlagStrMapNo, d->mapNo, d->unk1E3C, d->unk1E40, d->unk1E44, d->unk1E48);
    DbgPrintf(4, 6, dbgMainFlagStrOldMapNo, d->oldMapNo);
    DbgPrintf(4, 7, dbgMainFlagStrPartyId, d->party[0], d->party[1], d->party[2], d->party[3]);
    DbgPrintf(4, 8, dbgMainFlagStrRetryCounter, d->retryCount);
    DbgPrintf(4, 9, dbgMainFlagStrAreaMapNo, d->areaMapNo);
    DbgPrintf(4, 10, dbgMainFlagStrMainFlag, d->mainFlag);
    DbgPrintf(4, 11, dbgMainFlagStrEncount, dbgFldOffOnStr[EvtSystemFlagGet(0x64) ^ 1]);
    DbgPrintf(4, 12, dbgMainFlagStrTenantMove, dbgFldOffOnStr[EvtSystemFlagGet(0x70) ^ 1]);
    DbgPrintf(4, 13, dbgMainFlagStrMonsterMove, dbgFldOffOnStr[EvtSystemFlagGet(0x71) ^ 1]);
    DbgPrintf(4, 14, dbgMainFlagStrTenantDisp, dbgFldOffOnStr[EvtSystemFlagGet(0x72) ^ 1]);
    DbgPrintf(4, 15, dbgMainFlagStrMonsterDisp, dbgFldOffOnStr[EvtSystemFlagGet(0x73) ^ 1]);
    DbgPrintf(4, 16, dbgMainFlagStrCameraPause, dbgFldRunPauseStr[EvtSystemFlagGet(0x82)]);
    DbgPrintf(4, 17, dbgMainFlagStrCameraStatus, dbgFldStopRunStr[EvtSystemFlagGet(0x83)]);
    DbgPrintf(4, 18, dbgMainFlagStrDemoStatus, dbgFldOffOnStr[EvtSystemFlagGet(0x84)]);
    DbgPrintf(4, 19, dbgMainFlagStrDemoTempDisp, dbgFldOffOnStr[EvtSystemFlagGet(0xFD)]);
    DbgPrintf(0x1D, 8, dbgMainFlagStrOcc, dbgFldOffOnStr[EvtSystemFlagGet(0x6F) ^ 1]);
    DbgPrintf(0x1D, 9, dbgMainFlagStrMealEnd, dbgFldOffOnStr[EvtSystemFlagGet(0x60)]);
}
