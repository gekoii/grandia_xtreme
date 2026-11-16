#include "common.h"
#include "pad.h"
#include "main/n03.h"
#include "protos/main.h"

extern u8 mcDbgTitleStr[];
extern s32 mcDbgPrevPage;
extern s32 mcDbgPageChanged;


void McDbgMenu(void) {
    mcDbgPad = padPort0;
    mcDbgPadTrig = &padPort0[1];
    if (mcDbgPrevPage != mcDbgPage) {
        mcDbgPageChanged = 1;
    } else {
        mcDbgPageChanged = 0;
    }
    switch (mcDbgPage) {
    case 0:
        McDbgTopMenu();
        DbgPrintf(2, 1, mcDbgTitleStr);
        break;
    case 1:
        McDbgCmdPage();
        break;
    }
}
