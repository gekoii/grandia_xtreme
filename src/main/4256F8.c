#include "common.h"
#include "pad.h"
#include "main/n02.h"
#include "protos/main.h"

extern s32 dbgFilePage;
extern s32 dbgFileMenuStrFileMenuPage3;

void DbgFileMenu(void) {
    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        if ((padPort1.trigger & 4) && dbgFilePage != 0) {
            dbgFilePage--;
        }
        if ((dbgPad->trigger & 8) && dbgFilePage < 2) {
            dbgFilePage++;
        }
    }
    switch (dbgFilePage) {
    case 0:
        DbgFileInfoDraw();
        break;
    case 1:
        DbgFileCountsDraw();
        break;
    case 2:
        DbgFileIopStatDraw();
        break;
    }
    DbgPrintf(2, 3, &dbgFileMenuStrFileMenuPage3, dbgStrPageHiliteOn, dbgFilePage + 1, dbgStrPageHiliteOff);
}
