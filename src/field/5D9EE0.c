#include "common.h"
#include "pad.h"
#include "save.h"
#include "protos/field.h"
#include "field/n16.h"

extern s32 dbgFldXvsInfoLine;
extern u8 dbgXvsStrInfo1[];
extern u8 dbgXvsStrXvs[];
extern u8 dbgXvsStrBaffur[];
extern u8 dbgXvsStrMainCounter[];
extern u8 dbgXvsStrDebug1[];
extern u8 dbgXvsStrDebug2[];
extern u8 dbgXvsStrDebug3[];
extern u8 dbgXvsStrDebug4[];
extern u8 dbgXvsStrErrMsg[];
extern u8 dbgXvsStrEffect[];
extern u8 dbgXvsStrNonDemo[];

void DbgFldXvsInfoPage(void) {
    Pad *pad = &padPort1;

    if (!(padPort1.held & 2)) {
        dbgFldXvsInfoLine = DbgMenuLineSelect(dbgFldXvsInfoLine, 9);
        if (pad->repeatFast & 0x8000) {
            switch (dbgFldXvsInfoLine) {
            case 0:
                EvtSystemFlagSet(0x88, 0);
                break;
            case 1:
                EvtGlobalFlagSet(0x6E0, 0);
                break;
            case 2:
                sysSaveData.xvsCount--;
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                EvtGlobalFlagSet(dbgFldXvsInfoLine + 0x6DE, 0);
                break;
            }
        }
        if (pad->repeatFast & 0x2000) {
            switch (dbgFldXvsInfoLine) {
            case 0:
                EvtSystemFlagSet(0x88, 1);
                break;
            case 1:
                EvtGlobalFlagSet(0x6E0, 1);
                break;
            case 2:
                sysSaveData.xvsCount++;
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                EvtGlobalFlagSet(dbgFldXvsInfoLine + 0x6DE, 1);
                break;
            }
        }
    }
    DbgDrawCursor(3, dbgFldXvsInfoLine + 4);
    DbgPrintf(0x20, 4, dbgXvsStrInfo1);
    DbgPrintf(4, 4, dbgXvsStrXvs, dbgFldOffOnStr[EvtSystemFlagGet(0x88)]);
    DbgPrintf(4, 5, dbgXvsStrBaffur, dbgFldOffOnStr[EvtGlobalFlagGet(0x6E0)]);
    DbgPrintf(4, 6, dbgXvsStrMainCounter, sysSaveData.xvsCount);
    DbgPrintf(4, 7, dbgXvsStrDebug1, dbgFldOffOnStr[EvtGlobalFlagGet(0x6E1)]);
    DbgPrintf(4, 8, dbgXvsStrDebug2, dbgFldOffOnStr[EvtGlobalFlagGet(0x6E2)]);
    DbgPrintf(4, 9, dbgXvsStrDebug3, dbgFldOffOnStr[EvtGlobalFlagGet(0x6E3)]);
    DbgPrintf(4, 10, dbgXvsStrDebug4, dbgFldOffOnStr[EvtGlobalFlagGet(0x6E4)]);
    DbgPrintf(4, 11, dbgXvsStrErrMsg, dbgFldOffOnStr[EvtGlobalFlagGet(0x6E5)]);
    DbgPrintf(4, 12, dbgXvsStrEffect, dbgFldOffOnStr[EvtGlobalFlagGet(0x6E6)]);
    DbgPrintf(4, 13, dbgXvsStrNonDemo, dbgFldOffOnStr[EvtGlobalFlagGet(0x6E7)]);
}
