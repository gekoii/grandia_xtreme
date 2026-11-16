#include "common.h"
#include "pad.h"
#include "protos/main.h"


extern s32 dbgWipeType;
extern s32 dbgWipeParam0;
extern s32 dbgWipeParam1;
extern s32 dbgWipeLine;
extern s32 dbgWipeCursor;
extern char dbgWipeStrWipe[];
extern char dbgWipeStrType[];
extern char dbgWipeStrParam0[];
extern char dbgWipeStrParam1[];

void DbgWipePage(void) {
    s32 c2 = 3;
    s32 c3 = 5;
    s32 c4 = 3;
    s32 c5 = 6;
    s32 c6 = 3;
    s32 c7 = 7;
    dbgPad = &padPort1;
    DbgMenuCursorDraw(2, dbgWipeCursor + 5);
    if (!(dbgPad->held & 2)) {
        dbgWipeLine = DbgMenuLineSelect(dbgWipeLine, 3);
        switch (dbgWipeLine) {
        case 0:
            dbgWipeCursor = dbgWipeLine;
            if ((dbgPad->repeatFast & 0x8000) && dbgWipeType != 0) {
                dbgWipeType--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgWipeType++;
            }
            break;
        case 1:
            dbgWipeCursor = dbgWipeLine;
            if ((dbgPad->repeatFast & 0x8000) && dbgWipeParam0 != 0) {
                dbgWipeParam0--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgWipeParam0++;
            }
            break;
        case 2:
            dbgWipeCursor = dbgWipeLine;
            if ((dbgPad->repeatFast & 0x8000) && dbgWipeParam1 != 0) {
                dbgWipeParam1--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                dbgWipeParam1++;
            }
            break;
        }
        if (dbgPad->repeatFast & 0x20) {
            GphWipeStart(dbgWipeType, dbgWipeParam0, dbgWipeParam1);
        }
    }
    DbgPrintf(2, 3, dbgWipeStrWipe);
    DbgPrintf(c2, c3, dbgWipeStrType, dbgWipeType);
    DbgPrintf(c4, c5, dbgWipeStrParam0, dbgWipeParam0);
    DbgPrintf(c6, c7, dbgWipeStrParam1, dbgWipeParam1);
}
