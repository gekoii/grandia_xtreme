#include "common.h"
#include "pad.h"
#include "save.h"
#include "protos/field.h"



extern s32 dbgFldXvsCharaLine;
extern char dbgXvsStrInfo2[];
extern char dbgXvsStrLutina[];
extern char dbgXvsStrMyamm[];
extern char dbgXvsStrBrandol[];
extern char dbgXvsStrCarmyne[];
extern char dbgXvsStrJaid[];
extern char dbgXvsStrTitto[];
extern char dbgXvsStrUlk[];
extern char dbgXvsStrBoy[];
extern char dbgXvsStrOkusan[];
extern char dbgXvsStrOssan[];
extern char dbgXvsStrWaitress[];
extern char dbgXvsStrInu[];
extern char dbgXvsStrMagic1[];
extern char dbgXvsStrMagic2[];
extern char dbgXvsStrMuda[];
extern char dbgXvsStrSkill[];


void DbgFldXvsCharaPage(void) {
    Pad *pad;
    u16 btn;
    SysSaveData *g;

    pad = &padPort1;
    if (!(padPort1.held & 2)) {
        dbgFldXvsCharaLine = DbgMenuLineSelect(dbgFldXvsCharaLine, 31);
        btn = pad->repeatFast;
        if (btn & 0x8000) {
            sysSaveData.xvsCharA[dbgFldXvsCharaLine]--;
        }
        if (btn & 0x2000) {
            sysSaveData.xvsCharA[dbgFldXvsCharaLine]++;
        }
    }
    if (dbgFldXvsCharaLine < 16) {
        DbgDrawCursor(18, dbgFldXvsCharaLine + 4);
    } else {
        DbgDrawCursor(22, dbgFldXvsCharaLine - 12);
    }
    DbgPrintf(32, 4, dbgXvsStrInfo2);
    g = &sysSaveData;
    DbgPrintf(4, 4, dbgXvsStrLutina, g->xvsCharA[0], g->xvsCharB[0]);
    DbgPrintf(4, 5, dbgXvsStrMyamm, g->xvsCharA[1], g->xvsCharB[1]);
    DbgPrintf(4, 6, dbgXvsStrBrandol, g->xvsCharA[2], g->xvsCharB[2]);
    DbgPrintf(4, 7, dbgXvsStrCarmyne, g->xvsCharA[3], g->xvsCharB[3]);
    DbgPrintf(4, 8, dbgXvsStrJaid, g->xvsCharA[4], g->xvsCharB[4]);
    DbgPrintf(4, 9, dbgXvsStrTitto, g->xvsCharA[5], g->xvsCharB[5]);
    DbgPrintf(4, 10, dbgXvsStrUlk, g->xvsCharA[6], g->xvsCharB[6]);
    DbgPrintf(4, 11, dbgXvsStrBoy, g->xvsCharA[7], g->xvsCharB[7]);
    DbgPrintf(4, 12, dbgXvsStrOkusan, g->xvsCharA[8], g->xvsCharB[8]);
    DbgPrintf(4, 13, dbgXvsStrOssan, g->xvsCharA[9], g->xvsCharB[9]);
    DbgPrintf(4, 14, dbgXvsStrWaitress, g->xvsCharA[10], g->xvsCharB[10]);
    DbgPrintf(4, 15, dbgXvsStrInu, g->xvsCharA[11], g->xvsCharB[11]);
    DbgPrintf(4, 16, dbgXvsStrMagic1, g->xvsCharA[12], g->xvsCharB[12]);
    DbgPrintf(4, 17, dbgXvsStrMagic2, g->xvsCharA[13], g->xvsCharB[13]);
    DbgPrintf(4, 18, dbgXvsStrMuda, g->xvsCharA[14], g->xvsCharB[14]);
    DbgPrintf(4, 19, dbgXvsStrSkill, g->xvsCharA[15], g->xvsCharB[15]);
}
