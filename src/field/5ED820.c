#include "common.h"
#include "pad.h"
#include "field/n18.h"
#include "protos/field.h"

typedef struct {
    s32 sel;
    s32 mode;
} Cursor;

extern char dbgParamTitleFmt[];
extern s32 dbgParamPage;
extern char dbgParamPageNames[][12];
extern u8 dbgParamPageLimits[][2];
extern Cursor dbgParamCursors[];

void DbgParamUpdate(Pad *pad) {
    s32 flag = 0;
    s32 dir = 0;

    if (!(pad->held & 2)) {
        if (pad->trigger & 4) {
            if (dbgParamPage != 0) {
                dbgParamPage--;
            }
        }
        if (pad->trigger & 8) {
            if (dbgParamPage < 19) {
                dbgParamPage++;
            }
        }
        if (pad->repeatFast & 0x20) {
            flag = 1;
        }
        if (pad->held & 0x80) {
            dir = 0;
            if (pad->repeatFast & 0x8000) {
                dir = -1;
            }
            if (pad->repeatFast & 0x2000) {
                dir = 1;
            }
        } else {
            if (pad->repeatFast & 0x1000) {
                if (dbgParamCursors[dbgParamPage].sel != 0) {
                    dbgParamCursors[dbgParamPage].sel--;
                } else {
                    dbgParamCursors[dbgParamPage].sel = dbgParamPageLimits[dbgParamPage][0];
                }
            }
            if (pad->repeatFast & 0x4000) {
                if (dbgParamCursors[dbgParamPage].sel < dbgParamPageLimits[dbgParamPage][0]) {
                    dbgParamCursors[dbgParamPage].sel++;
                } else {
                    dbgParamCursors[dbgParamPage].sel = 0;
                }
            }
            if (pad->repeatFast & 0x8000) {
                if (dbgParamCursors[dbgParamPage].mode != 0) {
                    dbgParamCursors[dbgParamPage].mode--;
                } else {
                    dbgParamCursors[dbgParamPage].mode = dbgParamPageLimits[dbgParamPage][1];
                }
            }
            if (pad->repeatFast & 0x2000) {
                if (dbgParamCursors[dbgParamPage].mode < dbgParamPageLimits[dbgParamPage][1]) {
                    dbgParamCursors[dbgParamPage].mode++;
                } else {
                    dbgParamCursors[dbgParamPage].mode = 0;
                }
            }
        }
    }
    DbgPrintf(dbgParamX, dbgParamY - 2, dbgParamTitleFmt, dbgParamPage, 19, dbgParamPageNames[dbgParamPage]);
    switch (dbgParamPage) {
    case 0:
        DbgParamItemGetPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir, flag);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        DbgParamCharPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir, dbgParamPage - 1);
        break;
    case 9:
        DbgParamItemPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir);
        break;
    case 10:
        DbgParamValuablePage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir);
        break;
    case 11:
        DbgParamBookPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir);
        break;
    case 12:
        DbgParamSkillPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir, 0);
        break;
    case 13:
        DbgParamSkillPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir, 1);
        break;
    case 14:
        DbgParamSkillPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir, 2);
        break;
    case 15:
        DbgParamEggPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir, 0);
        break;
    case 16:
        DbgParamEggPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir, 1);
        break;
    case 17:
        DbgParamSpecialPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir, 0, flag);
        break;
    case 18:
        DbgParamSpecialPage(pad, dbgParamCursors[dbgParamPage].sel, dbgParamCursors[dbgParamPage].mode, dir, 1, flag);
        break;
    }
}
