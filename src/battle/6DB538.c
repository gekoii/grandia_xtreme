/* rodata: 0x006E2B80-0x006E2C08 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "sys.h"



static const char save[] = "save";


void DbgBtlCharMenu(void) {
    SysState *m;
    Pad *pad;

    switch (sysState.dbgMenuFlags & 0xF000) {
    case 0x1000:
        DbgBtlCharWorkPage();
        return;
    case 0x2000:
        DbgBtlCharParamPage();
        return;
    case 0x3000:
        DbgBtlPcSavePage();
        return;
    case 0x4000:
        DbgBtlParTestPage();
        return;
    case 0x5000:
        DbgBtlIpStopPage();
        return;
    }
    m = &sysState;
    if (!(m->dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        DbgBtlLineSelect(pad, 4);
        if (pad->trigger & 0x20) {
            switch (m->dbgCursor) {
            case 0:
                sysState.dbgPage = 0; sysState.dbgCursor = 0; sysState.dbgMenuFlags |= 0x1000;
                break;
            case 1:
                sysState.dbgPage = 0; sysState.dbgCursor = 0; sysState.dbgMenuFlags |= 0x2000;
                break;
            case 2:
                sysState.dbgPage = 0; sysState.dbgCursor = 0; sysState.dbgMenuFlags |= 0x3000;
                break;
            case 3:
                sysState.dbgPage = 0; sysState.dbgCursor = 0; sysState.dbgMenuFlags |= 0x4000;
                break;
            case 4:
                sysState.dbgPage = 0; sysState.dbgCursor = 0; sysState.dbgMenuFlags |= 0x5000;
                break;
            }
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, "%sCHAR%s", "\\c1 ", "\\c0 ");
    DbgPrintf(2, 3, "> WORK");
    DbgPrintf(2, 4, "> PARAM");
    DbgPrintf(2, 5, "> PC SAVE");
    DbgPrintf(2, 6, "> PAR TEST");
    DbgPrintf(2, 7, "> IP STOP");
}
