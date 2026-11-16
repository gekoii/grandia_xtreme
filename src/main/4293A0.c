#include "common.h"
#include "pad.h"
#include "main/n02.h"
#include "protos/main.h"

extern s32 dbgAtrPage;
extern s32 dbgAtrObjNo;
extern s32 dbgAtrMenuStrObjectAtrMenuPage3;

void DbgObjAtrMenu(void) {
    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        if ((padPort1.trigger & 4) && dbgAtrPage != 0) {
            dbgAtrPage--;
        }
        if ((dbgPad->trigger & 8) && dbgAtrPage <= 0) {
            dbgAtrPage++;
        }
    }
    switch (dbgAtrPage) {
    case 0:
        dbgAtrObjNo = DbgObjAtrPage(dbgAtrObjNo);
        break;
    case 1:
        DbgPartsAtrPage(dbgAtrObjNo);
        break;
    }
    DbgPrintf(2, 3, &dbgAtrMenuStrObjectAtrMenuPage3, dbgStrPageHiliteOn, dbgAtrPage + 1, dbgStrPageHiliteOff);
}
