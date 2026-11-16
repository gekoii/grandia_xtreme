/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "sys.h"
#include "battle/misc.h"

extern s8 dbgBtlTraceColorNo;
extern char dbgBtlTraceTitleFmt[];
extern char dbgBtlTraceTitleColorStr[];
extern char dbgBtlTraceColor0Str[];
extern char dbgBtlTraceNoFmt[];
extern char dbgBtlTraceStartRFmt[];
extern char dbgBtlTraceGFmt[];
extern char dbgBtlTraceBFmt[];
extern char dbgBtlTraceAFmt[];
extern char dbgBtlTraceEndRFmt[];

void DbgBtlTraceColorPage(void) {
    f32 one;
    s32 v;
    s8 *pd;
    FxTraceColor *c;
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, 8);
        switch (sysState.dbgCursor) {
        case 0:
            one = 1.0f;
            pd = &dbgBtlTraceColorNo;
            if (dbgBtlTraceColorNo + DbgEditValue(0, 0, 1, one) < 0.0f) {
                v = 10;
            } else if (dbgBtlTraceColorNo + DbgEditValue(0, 0, 1, one) > 10.0f) {
                v = 0;
            } else {
                v = dbgBtlTraceColorNo + DbgEditValue(0, 0, 1, one);
            }
            *pd = v;
            break;
        case 1:
            fxTraceColors[dbgBtlTraceColorNo].start.c.a += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 2:
            fxTraceColors[dbgBtlTraceColorNo].start.c.b += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 3:
            fxTraceColors[dbgBtlTraceColorNo].start.c.g += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 4:
            fxTraceColors[dbgBtlTraceColorNo].start.c.r += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 5:
            fxTraceColors[dbgBtlTraceColorNo].end.c.a += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 6:
            fxTraceColors[dbgBtlTraceColorNo].end.c.b += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 7:
            fxTraceColors[dbgBtlTraceColorNo].end.c.g += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 8:
            fxTraceColors[dbgBtlTraceColorNo].end.c.r += DbgEditValue(0, 0, 3, 1.0f);
            break;
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    c = fxTraceColors;
    DbgPrintf(2, 2, dbgBtlTraceTitleFmt, dbgBtlTraceTitleColorStr, dbgBtlTraceColor0Str);
    DbgPrintf(2, 3, dbgBtlTraceNoFmt, dbgBtlTraceColorNo);
    DbgPrintf(2, 4, dbgBtlTraceStartRFmt, c[dbgBtlTraceColorNo].start.c.a);
    DbgPrintf(5, 5, dbgBtlTraceGFmt, c[dbgBtlTraceColorNo].start.c.b);
    DbgPrintf(5, 6, dbgBtlTraceBFmt, c[dbgBtlTraceColorNo].start.c.g);
    DbgPrintf(5, 7, dbgBtlTraceAFmt, c[dbgBtlTraceColorNo].start.c.r);
    DbgPrintf(2, 8, dbgBtlTraceEndRFmt, c[dbgBtlTraceColorNo].end.c.a);
    DbgPrintf(5, 9, dbgBtlTraceGFmt, c[dbgBtlTraceColorNo].end.c.b);
    DbgPrintf(5, 10, dbgBtlTraceBFmt, c[dbgBtlTraceColorNo].end.c.g);
    DbgPrintf(5, 11, dbgBtlTraceAFmt, c[dbgBtlTraceColorNo].end.c.r);
}
