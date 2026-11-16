/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "sys.h"
#include "gph.h"
#include "protos/main.h"

extern s32 dbgFbEffLine;
extern s32 dbgFbEffCursor;
extern char dbgFbEffStrBlur[];
extern char dbgFbEffStrBoken[];
extern char dbgFbEffStrBlow[];
extern char dbgFbEffStrVwaveType[];
extern char dbgFbEffStrDiv[];
extern char dbgFbEffStrMax[];
extern char dbgFbEffStrAdd[];
extern char dbgFbEffStrCent[];
extern char dbgFbEffStrCnt[];
extern char dbgFbEffStrSpeed[];
extern char dbgFbEffStrColor[];
extern char dbgFbEffStrOverlapModeOn[];
extern char dbgFbEffStrOverlapModeOff[];
extern char dbgFbEffStrMultiModeOn[];
extern char dbgFbEffStrMultiModeOff[];
extern char dbgFbEffStrMultiW[];
extern char dbgFbEffStrMultiH[];

void DbgFbEffectPage(void) {
    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        switch (dbgFbEffLine = DbgMenuLineSelect(dbgFbEffLine, 14)) {
        case 0:
            dbgFbEffCursor = 0;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.blur--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.blur++;
            }
            break;
        case 1:
            dbgFbEffCursor = 1;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.bokeh -= 0.01f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.bokeh += 0.01f;
            }
            if (gphFbDivParam.bokeh < 0.0f) {
                gphFbDivParam.bokeh = 0.0f;
            }
            break;
        case 2:
            dbgFbEffCursor = 2;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.blow -= 0.001f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.blow += 0.001f;
            }
            if (gphFbDivParam.blow < 0.0f) {
                gphFbDivParam.blow = 0.0f;
            }
            break;
        case 3:
            dbgFbEffCursor = 3;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbEffParam[0]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbEffParam[0]++;
            }
            break;
        case 4:
            dbgFbEffCursor = 4;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbEffParam[1]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbEffParam[1]++;
            }
            break;
        case 5:
            dbgFbEffCursor = 5;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbEffParam[2]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbEffParam[2]++;
            }
            break;
        case 6:
            dbgFbEffCursor = 6;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbEffParam[3]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbEffParam[3]++;
            }
            break;
        case 7:
            dbgFbEffCursor = 7;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbEffParam[4]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbEffParam[4]++;
            }
            break;
        case 8:
            dbgFbEffCursor = 8;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbEffParam[5]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbEffParam[5]++;
            }
            break;
        case 9:
            dbgFbEffCursor = 9;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbEffParam[6]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbEffParam[6]++;
            }
            break;
        case 10:
            dbgFbEffCursor = 10;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbEffParam[7]--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbEffParam[7]++;
            }
            break;
        case 11:
            dbgFbEffCursor = 11;
            if (dbgPad->trigger & 0x2000) {
                sysState.screenCaptureReq = 1;
            }
            break;
        case 12:
            dbgFbEffCursor = 12;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.multiMode = 0;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.multiMode = 1;
            }
            break;
        case 13:
            dbgFbEffCursor = 13;
            if ((dbgPad->repeatFast & 0x8000) && gphFbDivParam.multiW >= 3) {
                gphFbDivParam.multiW--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.multiW++;
            }
            break;
        case 14:
            dbgFbEffCursor = 14;
            if ((dbgPad->repeatFast & 0x8000) && gphFbDivParam.multiH >= 3) {
                gphFbDivParam.multiH--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.multiH++;
            }
            break;
        }
    }
    DbgMenuCursorDraw(2, dbgFbEffCursor + 5);
    DbgPrintf(3, 5, dbgFbEffStrBlur, gphFbDivParam.blur);
    DbgPrintf(3, 6, dbgFbEffStrBoken, gphFbDivParam.bokeh);
    DbgPrintf(3, 7, dbgFbEffStrBlow, gphFbDivParam.blow);
    DbgPrintf(3, 8, dbgFbEffStrVwaveType, gphFbEffParam[0]);
    DbgPrintf(3, 9, dbgFbEffStrDiv, gphFbEffParam[1]);
    DbgPrintf(3, 10, dbgFbEffStrMax, gphFbEffParam[2]);
    DbgPrintf(3, 11, dbgFbEffStrAdd, gphFbEffParam[3]);
    DbgPrintf(3, 12, dbgFbEffStrCent, gphFbEffParam[4]);
    DbgPrintf(3, 13, dbgFbEffStrCnt, gphFbEffParam[5]);
    DbgPrintf(3, 14, dbgFbEffStrSpeed, gphFbEffParam[6]);
    DbgPrintf(3, 15, dbgFbEffStrColor, gphFbEffParam[7]);
    if (sysState.screenCaptureReq != 0) {
        DbgPrintf(3, 16, dbgFbEffStrOverlapModeOn);
    } else {
        DbgPrintf(3, 16, dbgFbEffStrOverlapModeOff);
    }
    if (gphFbDivParam.multiMode != 0) {
        DbgPrintf(3, 17, dbgFbEffStrMultiModeOn);
    } else {
        DbgPrintf(3, 17, dbgFbEffStrMultiModeOff);
    }
    DbgPrintf(3, 18, dbgFbEffStrMultiW, gphFbDivParam.multiW);
    DbgPrintf(3, 19, dbgFbEffStrMultiH, gphFbDivParam.multiH);
}
