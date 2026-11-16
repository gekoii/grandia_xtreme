#include "common.h"
#include "pad.h"
#include "gph.h"
#include "main/n02.h"
#include "protos/main.h"

extern u32 dbgFbSys2Line;
extern u32 dbgFbSys2Cursor;
extern char dbgFbSys2StrSystem2Mode[];
extern char dbgFbSys2StrX[];
extern char dbgFbSys2StrY[];
extern char dbgFbSys2StrW[];
extern char dbgFbSys2StrH[];
extern char dbgFbSys2StrU[];
extern char dbgFbSys2StrV[];
extern char dbgFbSys2StrAMode[];
extern char dbgFbSys2StrAlpha[];
extern char dbgFbSys2StrTheata[];
extern char dbgFbSys2StrP0R[];
extern char dbgFbSys2StrG[];
extern char dbgFbSys2StrB[];
extern char dbgFbSys2StrA[];

void DbgFbSystem2Page(void) {
    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        dbgFbSys2Line = DbgMenuLineSelect(dbgFbSys2Line, 0xE);
        switch (dbgFbSys2Line) {
        case 0:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.system2Mode = 0;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.system2Mode = 1;
            }
            break;
        case 1:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.x--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.x++;
            }
            break;
        case 2:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.y--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.y++;
            }
            break;
        case 3:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.w--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.w++;
            }
            break;
        case 4:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.h--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.h++;
            }
            break;
        case 5:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                if (gphFbDivParam.u != 0) {
                    gphFbDivParam.u--;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.u++;
            }
            break;
        case 6:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                if (gphFbDivParam.v != 0) {
                    gphFbDivParam.v--;
                }
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.v++;
            }
            break;
        case 7:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.aMode--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.aMode++;
            }
            break;
        case 8:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.alpha--;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.alpha++;
            }
            break;
        case 9:
            dbgFbSys2Cursor = dbgFbSys2Line;
            if (dbgPad->repeatFast & 0x8000) {
                gphFbDivParam.theta -= 1.0f;
            }
            if (dbgPad->repeatFast & 0x2000) {
                gphFbDivParam.theta += 1.0f;
            }
            break;
        }
    }
    DbgMenuCursorDraw(2, dbgFbSys2Cursor + 5);
    DbgPrintf(3, 4, dbgFbSys2StrSystem2Mode);
    if (gphFbDivParam.system2Mode != 0) {
        DbgPrintf(3, 5, dbgStrDispOn);
    } else {
        DbgPrintf(3, 5, dbgStrDispOff);
    }
    DbgPrintf(3, 6, dbgFbSys2StrX, gphFbDivParam.x);
    DbgPrintf(3, 7, dbgFbSys2StrY, gphFbDivParam.y);
    DbgPrintf(3, 8, dbgFbSys2StrW, gphFbDivParam.w);
    DbgPrintf(3, 9, dbgFbSys2StrH, gphFbDivParam.h);
    DbgPrintf(3, 0xA, dbgFbSys2StrU, gphFbDivParam.u);
    DbgPrintf(3, 0xB, dbgFbSys2StrV, gphFbDivParam.v);
    DbgPrintf(3, 0xC, dbgFbSys2StrAMode, gphFbDivParam.aMode);
    DbgPrintf(3, 0xD, dbgFbSys2StrAlpha, gphFbDivParam.alpha);
    DbgPrintf(3, 0xE, dbgFbSys2StrTheata, gphFbDivParam.theta);
    DbgPrintf(3, 0xF, dbgFbSys2StrP0R, gphFbDivParam.r[0]);
    DbgPrintf(3, 0xF, dbgFbSys2StrG, gphFbDivParam.g[0]);
    DbgPrintf(3, 0x10, dbgFbSys2StrB, gphFbDivParam.b[0]);
    DbgPrintf(3, 0x11, dbgFbSys2StrA, gphFbDivParam.a[0]);
}
