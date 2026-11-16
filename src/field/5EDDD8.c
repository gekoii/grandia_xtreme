#include "common.h"
#include "field/n18.h"
#include "protos/field.h"

typedef struct {
    u16 min;
    u16 max;
} Range;

typedef struct {
    Range r[4];
} Ranges;

s32 ItemCanAdd(s16);

extern s32 dbgParamItemGetInit;
extern s8 amapForceAlgo1;
extern s8 amapForceAlgo2;
extern s8 amapForceTemplates[];
extern Ranges dbgParamItemGetRanges;
extern char dbgParamStrItemNo[];
extern char dbgParamStrEggNo[];
extern char dbgParamStrBookNo[];
extern char dbgParamStrSkillNo[];
extern char dbgParamStrLeaveItem[];
extern char dbgParamStrGold[];
extern char dbgParamStrTempMode[];
extern char dbgParamStrTempMode2[];
extern char dbgParamStrTempNo[];
extern char dbgParamStrItemOk[];
extern char dbgParamStrItemErr[];
extern char dbgParamStrEggOk[];
extern char dbgParamStrEggErr[];
extern char dbgParamStrBookOk[];
extern char dbgParamStrBookErr[];
extern char dbgParamStrSkillOk[];
extern char dbgParamStrSkillErr[];
extern char dbgParamStrLeaveOk[];
extern s32 dbgParamEggNo;
extern s32 dbgParamSkillNo;
extern s32 dbgParamMsgTimer;
extern s32 dbgParamBookNo;
extern s32 dbgParamResult;
extern s32 dbgParamItemNo;

void DbgParamItemGetPage(s32 arg0, u32 sel, s32 arg2, s32 dir, s32 exec) {
    Ranges lim;
    s32 x;
    s32 y;
    s32 i;

    lim = dbgParamItemGetRanges;
    if (dbgParamItemGetInit == 0) {
        dbgParamItemNo = lim.r[0].min;
        dbgParamEggNo = lim.r[1].min;
        dbgParamBookNo = lim.r[2].min;
        dbgParamSkillNo = lim.r[3].min;
        dbgParamItemGetInit = 1;
    }
    DbgDrawCursor(dbgParamX - 1, dbgParamY + sel);
    x = dbgParamX;
    y = dbgParamY;
    DbgPrintf(x, y, dbgParamStrItemNo, dbgParamItemNo);
    DbgPrintf(x, y + 1, dbgParamStrEggNo, dbgParamEggNo);
    DbgPrintf(x, y + 2, dbgParamStrBookNo, dbgParamBookNo);
    DbgPrintf(x, y + 3, dbgParamStrSkillNo, dbgParamSkillNo);
    DbgPrintf(x, y + 4, dbgParamStrLeaveItem);
    DbgPrintf(x, y + 5, dbgParamStrGold, PtyGetGold());
    DbgPrintf(x, y + 6, dbgParamStrTempMode, amapForceAlgo1);
    DbgPrintf(x, y + 7, dbgParamStrTempMode2, amapForceAlgo2);
    for (i = 0; i < 9; i++) {
        DbgPrintf(x, y + 8 + i, dbgParamStrTempNo, i + 1, amapForceTemplates[i + 1]);
    }
    {
        switch (sel) {
        case 0:
            if (dir == 1) {
                if (dbgParamItemNo < lim.r[0].max) {
                    dbgParamItemNo++;
                } else {
                    break;
                }
            }
            if (dir == -1 && lim.r[0].min < dbgParamItemNo) {
                dbgParamItemNo--;
            }
            break;
        case 1:
            if (dir == 1) {
                if (dbgParamEggNo < lim.r[1].max) {
                    dbgParamEggNo++;
                } else {
                    break;
                }
            }
            if (dir == -1 && lim.r[1].min < dbgParamEggNo) {
                dbgParamEggNo--;
            }
            break;
        case 2:
            if (dir == 1) {
                if (dbgParamBookNo < lim.r[2].max) {
                    dbgParamBookNo++;
                } else {
                    break;
                }
            }
            if (dir == -1 && lim.r[2].min < dbgParamBookNo) {
                dbgParamBookNo--;
            }
            break;
        case 3:
            if (dir == 1) {
                if (dbgParamSkillNo < lim.r[3].max) {
                    dbgParamSkillNo++;
                } else {
                    break;
                }
            }
            if (dir == -1 && lim.r[3].min < dbgParamSkillNo) {
                dbgParamSkillNo--;
            }
            break;
        case 5:
            if (dir == 1) {
                PtySetGold(PtyGetGold() + 10);
            }
            if (dir == -1) {
                PtySetGold(PtyGetGold() - 10);
            }
            break;
        case 6:
            if (dir == 1) {
                amapForceAlgo1++;
            }
            if (dir == -1) {
                amapForceAlgo1--;
            }
            break;
        case 7:
            if (dir == 1) {
                amapForceAlgo2++;
            }
            if (dir == -1) {
                amapForceAlgo2--;
            }
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            if (dir == 1) {
                amapForceTemplates[sel - 7]++;
            }
            if (dir == -1) {
                amapForceTemplates[sel - 7]--;
            }
            break;
        }
    }
    {
        switch (sel) {
        case 0:
            if (exec) {
                if (ItemCanAdd(dbgParamItemNo) >= 0 && ItemAdd(dbgParamItemNo) >= 0) {
                    dbgParamResult = 1;
                } else {
                    dbgParamResult = 2;
                }
                dbgParamMsgTimer = 60;
            }
            break;
        case 1:
            if (exec) {
                if (ItemCanAdd(dbgParamEggNo) >= 0 && ItemAdd(dbgParamEggNo) >= 0) {
                    dbgParamResult = 11;
                } else {
                    dbgParamResult = 12;
                }
                dbgParamMsgTimer = 60;
            }
            break;
        case 2:
            if (exec) {
                if (ItemCanAdd(dbgParamBookNo) >= 0 && ItemAdd(dbgParamBookNo) >= 0) {
                    dbgParamResult = 21;
                } else {
                    dbgParamResult = 22;
                }
                dbgParamMsgTimer = 60;
            }
            break;
        case 3:
            if (exec) {
                dbgParamMsgTimer = 60;
                dbgParamResult = 32;
                if (ItemSkillAdd(dbgParamSkillNo) >= 0) {
                    dbgParamResult = 31;
                }
            }
            break;
        case 4:
            if (exec) {
                for (i = 0; i < 0x800; i++) {
                    ItemStorageAdd(i, 100);
                }
                dbgParamResult = 40;
                dbgParamMsgTimer = 60;
            }
            break;
        }
    }
    if (dbgParamMsgTimer != 0) {
        switch (dbgParamResult) {
        case 1:
            DbgPrintf(x + 20, y + sel, dbgParamStrItemOk);
            break;
        case 2:
            DbgPrintf(x + 20, y + sel, dbgParamStrItemErr);
            break;
        case 11:
            DbgPrintf(x + 20, y + sel, dbgParamStrEggOk);
            break;
        case 12:
            DbgPrintf(x + 20, y + sel, dbgParamStrEggErr);
            break;
        case 21:
            DbgPrintf(x + 20, y + sel, dbgParamStrBookOk);
            break;
        case 22:
            DbgPrintf(x + 20, y + sel, dbgParamStrBookErr);
            break;
        case 31:
            DbgPrintf(x + 20, y + sel, dbgParamStrSkillOk);
            break;
        case 32:
            DbgPrintf(x + 20, y + sel, dbgParamStrSkillErr);
            break;
        case 40:
            DbgPrintf(x + 20, y + sel, dbgParamStrLeaveOk);
            break;
        }
        dbgParamMsgTimer--;
    }
}
