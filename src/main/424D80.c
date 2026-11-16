#include "common.h"
#include "pad.h"
#include "main/n02.h"
#include "protos/main.h"

extern s32 dbgFbPage;
extern u8 dbgFbMenuStrFramebufferMenu3[];

void DbgFbMenu(void) {
    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        if ((padPort1.trigger & 4) && dbgFbPage != 0) {
            dbgFbPage--;
        }
        if ((dbgPad->trigger & 8) && dbgFbPage < 2) {
            dbgFbPage++;
        }
    }
    switch (dbgFbPage) {
    case 0:
        DbgFbEffectPage();
        break;
    case 1:
        DbgFbDividePage();
        break;
    case 2:
        DbgFbSystem2Page();
        break;
    }
    DbgPrintf(2, 3, dbgFbMenuStrFramebufferMenu3, dbgStrPageHiliteOn, dbgFbPage + 1, dbgStrPageHiliteOff);
}
