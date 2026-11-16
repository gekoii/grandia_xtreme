/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"
#include "battle/n34.h"

extern char dbgBtlParTestTitleFmt[];
extern char dbgBtlParTestMovFmt[];
extern char dbgBtlParTestColRFmt[];
extern char dbgBtlParTestColHFmt[];
extern char dbgBtlParTestReach1Fmt[];
extern char dbgBtlParTestScope1Fmt[];
extern char dbgBtlParTestReach2Fmt[];
extern char dbgBtlParTestScope2Fmt[];

void DbgBtlParTestPage(void) {
    BtlUnit *u;
    PtyUnit *st;
    s16 *p4 = NULL;
    s16 *p5 = NULL;
    s16 *p6 = NULL;
    s16 *p7 = NULL;
    PtyMove *e;
    BtlAction *m;

    u = &btlUnits[dbgBtlCharNo];
    st = u->params;
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        DbgBtlLineSelect(&padPort1, u->id <= 3 ? 7 : 5);
        if (sysState.dbgCursor == 0) {
            dbgBtlCharNo = (dbgBtlCharNo + DbgEditValue(0, 0, 1, 1.0f) < 0.0f) ? 15
                       : (dbgBtlCharNo + DbgEditValue(0, 0, 1, 1.0f) > 15.0f) ? 0
                       : dbgBtlCharNo + DbgEditValue(0, 0, 1, 1.0f);
        }
        u = &btlUnits[dbgBtlCharNo];
        st = u->params;
    }
    if (u->state > 0) {
        if (u->id < 4) {
            e = &ptyMoveTable[u->def->id - 3916];
            p6 = &e->reach;
            p7 = &e->levels->scope;
            e = &ptyMoveTable[u->def->id - 3906];
            p4 = &e->reach;
            p5 = &e->levels->scope;
        } else {
            m = u->def->data->actionSets->actions[0];
            p6 = p4 = &m->reach;
            p7 = p5 = &m->level->scope;
        }
        if (!(sysState.dbgMenuFlags & 0x20000000)) {
            switch (sysState.dbgCursor) {
            case 1:
                st->params[5] = st->params[5] + DbgEditValue(0, 0, 3, 10.0f);
                break;
            case 2:
                st->colR += DbgEditValue(2, 0, 3, 10.0f);
                break;
            case 3:
                st->colH += DbgEditValue(2, 0, 3, 10.0f);
                break;
            case 4:
                *p6 = *p6 + DbgEditValue(0, 0, 3, 10.0f);
                break;
            case 5:
                *p7 = *p7 + DbgEditValue(0, 0, 3, 10.0f);
                break;
            case 6:
                *p4 = *p4 + DbgEditValue(0, 0, 3, 10.0f);
                break;
            case 7:
                *p5 = *p5 + DbgEditValue(0, 0, 3, 10.0f);
                break;
            }
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, dbgBtlParTestTitleFmt, dbgBtlColor1Str, dbgBtlColor0Str);
    DbgPrintf(2, 3, dbgBtlCharNoFmt, dbgBtlCharNo);
    if (u->state > 0) {
        DbgPrintf(5, 4, dbgBtlParTestMovFmt, st->params[5]);
        DbgPrintf(3, 5, dbgBtlParTestColRFmt, st->colR);
        DbgPrintf(7, 6, dbgBtlParTestColHFmt, st->colH);
        DbgPrintf(2, 7, dbgBtlParTestReach1Fmt, *p6);
        DbgPrintf(2, 8, dbgBtlParTestScope1Fmt, *p7);
        if (u->id < 4) {
            DbgPrintf(2, 9, dbgBtlParTestReach2Fmt, *p4);
            DbgPrintf(2, 10, dbgBtlParTestScope2Fmt, *p5);
        }
    }
}
