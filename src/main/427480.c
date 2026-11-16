#include "common.h"
#include "pad.h"
#include "gph.h"
#include "main/n02.h"
#include "protos/main.h"

extern s8 dbgMotPartsSel;
extern s32 dbgMotPartsTop;
extern s32 dbgMotPartsCursor;
extern u8 dbgMotPartsStrDispOff[];
extern u8 dbgMotPartsStrDispOn[];
extern u8 dbgMotPartsStrPartsListMax[];
extern u8 dbgMotPartsStrEnd[];

void DbgMotionPartsPage(void) {
    GphModel *list;
    s32 cnt;
    GphPart *e;
    s32 idx;
    s32 i;

    cnt = 0;
    list = dbgObjList->objs[dbgObjNo];
    dbgPad = &padPort1;
    if (list != NULL) {
        ChrModelGetPart(list, dbgMotPartsSel);
    }
    DbgMenuCursorDraw(2, dbgMotPartsCursor + 6);
    if (list != NULL) {
        while (ChrModelGetPart(list, cnt) != NULL) {
            cnt++;
        }
    }
    if (!(dbgPad->held & 2)) {
        if (dbgPad->repeatFast & 0x1000) {
            if (dbgMotPartsTop != 0) {
                if (dbgMotPartsCursor >= 7) {
                    dbgMotPartsCursor--;
                } else {
                    dbgMotPartsTop--;
                }
            } else if (dbgMotPartsCursor != 0) {
                dbgMotPartsCursor--;
            }
        }
        if (dbgPad->repeatFast & 0x4000) {
            if (dbgMotPartsCursor < 6) {
                dbgMotPartsCursor++;
            } else if (dbgMotPartsTop + 13 < cnt) {
                dbgMotPartsTop++;
            } else if (dbgMotPartsTop + dbgMotPartsCursor < cnt - 1) {
                dbgMotPartsCursor++;
            }
        }
        if (dbgPad->trigger & 0x20) {
            if (list != NULL) {
                e = ChrModelGetPart(list, dbgMotPartsTop + dbgMotPartsCursor);
                if (e != NULL) {
                    e->flags ^= 8;
                }
            }
        }
    }
    if (list != NULL) {
        idx = dbgMotPartsTop;
        i = 0;
        while ((e = ChrModelGetPart(list, idx)) != NULL && i < 13) {
            if (e->flags & 8) {
                DbgPrintf(3, i + 6, dbgMotPartsStrDispOff, idx);
            } else {
                DbgPrintf(3, i + 6, dbgMotPartsStrDispOn, idx);
            }
            idx++;
            i++;
        }
    }
    DbgPrintf(3, 5, dbgMotPartsStrPartsListMax, cnt);
    if (dbgMotPartsTop + 13 == cnt) {
        DbgPrintf(3, 0x13, dbgMotPartsStrEnd);
    }
}
