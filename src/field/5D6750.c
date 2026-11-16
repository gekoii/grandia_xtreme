/* rodata: 0x006717C8-0x006717F8 */
#include "common.h"
#include "pad.h"
#include "protos/field.h"


extern s32 dbgFldSystemMenuPage;

void DbgFldSystemMenu(void) {
    Pad *pad;

    pad = &padPort1;
    DbgPrintf(2, 2, "SYSTEM MENU page %s %d/3%s ", "\\c5 ", dbgFldSystemMenuPage + 1, "\\c0 ");
    if (!(pad->held & 2)) {
        if ((pad->trigger & 4) && dbgFldSystemMenuPage != 0) {
            dbgFldSystemMenuPage--;
        }
        if ((pad->trigger & 8) && dbgFldSystemMenuPage < 2) {
            dbgFldSystemMenuPage++;
        }
    }
    switch (dbgFldSystemMenuPage) {
    case 0:
        DbgFldSystemPage();
        break;
    case 1:
        DbgFldFadePage();
        break;
    case 2:
        FldPointLightDebugPage();
        break;
    }
}
