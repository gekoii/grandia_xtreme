#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"



extern s32 dbgScrDivLine;
extern s32 dbgScrDivCursor;
extern char dbgScrDivStrScreenDiv[];
extern char dbgScrDivStrTypeNo[];
extern char dbgScrDivStrModeNo[];
extern char dbgScrDivStrCouterNo[];

void DbgScreenDivPage(void) {
    s32 r;

    dbgPad = &padPort1;
    DbgMenuCursorDraw(2, dbgScrDivCursor + 5);
    if (!(dbgPad->held & 2)) {
        r = DbgMenuLineSelect(dbgScrDivLine, 3);
        dbgScrDivLine = r;
        switch (r) {
        case 0:
            dbgScrDivCursor = 0;
            if (dbgPad->repeatFast & 0x8000) {
                if (sysState.unk63 != 0) {
                    sysState.unk63--;
                }
                sysState.unk66 = 0x140;
            }
            if (dbgPad->repeatFast & 0x2000) {
                sysState.unk66 = 0x140;
                sysState.unk63++;
            }
            break;
        case 1:
            dbgScrDivCursor = r;
            if (dbgPad->repeatFast & 0x8000) {
                sysState.unk64 = 0;
            }
            if (dbgPad->repeatFast & 0x2000) {
                sysState.unk64 = r;
            }
            break;
        }
    }
    DbgPrintf(2, 3, dbgScrDivStrScreenDiv);
    DbgPrintf(3, 5, dbgScrDivStrTypeNo, sysState.unk63);
    DbgPrintf(3, 6, dbgScrDivStrModeNo, sysState.unk64);
    DbgPrintf(3, 7, dbgScrDivStrCouterNo, sysState.unk66);
}
