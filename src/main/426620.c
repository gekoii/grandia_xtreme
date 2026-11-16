#include "common.h"
#include "pad.h"
#include "main/n02.h"
#include "protos/main.h"

extern s32 dbgSndPage;
extern char dbgSndMenuStrSoundMenuPage4[];

void DbgSoundMenu(void) {
    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        if ((dbgPad->trigger & 4) && dbgSndPage != 0) {
            dbgSndPage--;
        }
        if ((dbgPad->trigger & 8) && dbgSndPage < 3) {
            dbgSndPage++;
        }
    }
    switch (dbgSndPage) {
    case 0:
        DbgSoundBgmPage();
        break;
    case 1:
        DbgSoundSePage();
        break;
    case 2:
        DbgFileIopStatDraw();
        break;
    case 3:
        DbgSoundEqPage();
        break;
    }
    DbgPrintf(2, 3, dbgSndMenuStrSoundMenuPage4, dbgStrPageHiliteOn, dbgSndPage + 1, dbgStrPageHiliteOff);
}
