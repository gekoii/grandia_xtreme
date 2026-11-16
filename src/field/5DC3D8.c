#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/n16.h"

extern s32 dbgFldSetFileNo;
extern char dbgSetFileStrHeader[];
extern char dbgSetFileStrNo[];
extern char dbgSetFileStrGoldNum[];
extern char dbgSetFileStrGoldMax[];
extern char dbgSetFileStrGoldMin[];
extern char dbgSetFileStrTrapDmg3[];
extern char dbgSetFileStrTrapDmg2[];
extern char dbgSetFileStrTrapDmg1[];
extern char dbgSetFileStrBtlFld[];
extern char dbgSetFileStrItNum1[];
extern char dbgSetFileStrItRank1[];
extern char dbgSetFileStrItNum2[];
extern char dbgSetFileStrItRank2[];
extern char dbgSetFileStrMcNum[];
extern char dbgSetFileStrMapNo[];
extern char dbgSetFileStrMonsterHeader[];
extern char dbgSetFileStrMcNo[];
extern char dbgSetFileStrMcId[];
extern char dbgSetFileStrLvMax[];
extern char dbgSetFileStrLvMin[];
extern char dbgSetFileStrRate[];
extern char dbgSetFileStrNum[];
extern char dbgSetFileStrLeader[];
extern char dbgSetFileStrCmm[];

void DbgFldSetFilePage(void) {
    Pad *pad;
    MapSetFile *info;
    MapSetFileMonster *e;
    u16 btn;

    pad = &padPort1;
    info = MapSetFileGet();
    if (info != NULL) {
        DbgPrintf(4, 5, dbgSetFileStrHeader);
        DbgPrintf(4, 6, dbgSetFileStrNo, info->no);
        DbgPrintf(4, 7, dbgSetFileStrGoldNum, info->goldNum);
        DbgPrintf(4, 8, dbgSetFileStrGoldMax, info->goldMax);
        DbgPrintf(4, 9, dbgSetFileStrGoldMin, info->goldMin);
        DbgPrintf(4, 10, dbgSetFileStrTrapDmg3, info->trapDmg3);
        DbgPrintf(4, 11, dbgSetFileStrTrapDmg2, info->trapDmg2);
        DbgPrintf(4, 12, dbgSetFileStrTrapDmg1, info->trapDmg1);
        DbgPrintf(4, 13, dbgSetFileStrBtlFld, info->btlFld);
        DbgPrintf(24, 6, dbgSetFileStrItNum1, info->itNum1);
        DbgPrintf(24, 7, dbgSetFileStrItRank1, info->itRank1);
        DbgPrintf(24, 8, dbgSetFileStrItNum2, info->itNum2);
        DbgPrintf(24, 9, dbgSetFileStrItRank2, info->itRank2);
        DbgPrintf(24, 10, dbgSetFileStrMcNum, info->mcNum);
        DbgPrintf(24, 11, dbgSetFileStrGoldNum, info->goldNum);
        DbgPrintf(24, 12, dbgSetFileStrMapNo, info->mapNo);
        if (!(padPort1.held & 2)) {
            btn = pad->trigger;
            if (btn & 0x8000) {
                if (dbgFldSetFileNo != 0) {
                    dbgFldSetFileNo--;
                } else {
                    dbgFldSetFileNo = 15;
                }
            }
            if (btn & 0x2000) {
                dbgFldSetFileNo++;
            }
        }
        if (dbgFldSetFileNo >= 16) {
            dbgFldSetFileNo = 0;
        }
        e = &info->monsters[dbgFldSetFileNo];
        DbgDrawCursor(3, 15);
        DbgPrintf(4, 14, dbgSetFileStrMonsterHeader);
        DbgPrintf(4, 15, dbgSetFileStrMcNo, dbgFldSetFileNo + 1, 16);
        DbgPrintf(4, 16, dbgSetFileStrMcId, e->id);
        DbgPrintf(4, 17, dbgSetFileStrLvMax, e->lvMax);
        DbgPrintf(4, 18, dbgSetFileStrLvMin, e->lvMin);
        DbgPrintf(24, 15, dbgSetFileStrRate, e->rate);
        DbgPrintf(24, 16, dbgSetFileStrNum, e->num);
        DbgPrintf(24, 17, dbgSetFileStrLeader, e->leader);
        DbgPrintf(24, 18, dbgSetFileStrCmm, e->cmm);
    }
}
