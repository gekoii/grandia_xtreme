/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "gph.h"
#include "protos/main.h"

extern u32 dbgFbDivLine;
extern u32 dbgFbDivCursor;
extern char dbgFbDivStrCenterViewOn[];
extern char dbgFbDivStrCenterViewOff[];
extern char dbgFbDivStr9DivideOn[];
extern char dbgFbDivStr9DivideOff[];
extern char dbgFbDivStr9DivideUdScrl[];
extern char dbgFbDivStr9DivideLrScrl[];
extern char dbgFbDivStrMirrorOn[];
extern char dbgFbDivStrMirrorOff[];

void DbgFbDividePage(void) {
    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        dbgFbDivLine = DbgMenuLineSelect(dbgFbDivLine, 4);
        switch (dbgFbDivLine) {
        case 2:
        case 3:
            dbgFbDivCursor = dbgFbDivLine;
            if (dbgPad->repeatFast & 0x8000) {
                ((f32 *)&gphFbDivParam)[dbgFbDivLine] -= 0.1f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                ((f32 *)&gphFbDivParam)[dbgFbDivLine] += 0.1f;
            }
            if (((f32 *)&gphFbDivParam)[dbgFbDivLine] < 0.0f) {
                ((f32 *)&gphFbDivParam)[dbgFbDivLine] = 0.0f;
            }
            break;
        case 0:
        case 1:
        case 4:
            dbgFbDivCursor = dbgFbDivLine;
            if (dbgPad->repeatFast & 0x8000) {
                ((f32 *)&gphFbDivParam)[dbgFbDivLine] = 0.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                ((f32 *)&gphFbDivParam)[dbgFbDivLine] = 1.0f;
            }
            break;
        }
    }
    DbgMenuCursorDraw(2, dbgFbDivCursor + 5);
    if (gphFbDivParam.centerView == 1.0f) {
        DbgPrintf(3, 5, dbgFbDivStrCenterViewOn);
    } else {
        DbgPrintf(3, 5, dbgFbDivStrCenterViewOff);
    }
    if (gphFbDivParam.nineDivide == 1.0f) {
        DbgPrintf(3, 6, dbgFbDivStr9DivideOn);
    } else {
        DbgPrintf(3, 6, dbgFbDivStr9DivideOff);
    }
    DbgPrintf(3, 7, dbgFbDivStr9DivideUdScrl, gphFbDivParam.nineDivScrollV);
    DbgPrintf(3, 8, dbgFbDivStr9DivideLrScrl, gphFbDivParam.nineDivScrollH);
    if (gphFbDivParam.nineDivide == 1.0f) {
        DbgPrintf(3, 9, dbgFbDivStrMirrorOn);
    } else {
        DbgPrintf(3, 9, dbgFbDivStrMirrorOff);
    }
}
