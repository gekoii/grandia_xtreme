/* rodata: 0x006E2AC0-0x006E2B60 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "sys.h"
#include "battle/n34.h"

static const char save[] = "save";


void DbgBtlSysMenu(void) {
    SysState *m;
    Pad *pad;

    switch (sysState.dbgMenuFlags & 0xF000) {
    case 0x1000:
        DbgBtlCameraPage();
        return;
    case 0x2000:
        DbgBtlInfoPage();
        return;
    case 0x3000:
        DbgBtlTraceColorPage();
        return;
    case 0x4000:
        DbgBtlTraceLightPage();
        return;
    case 0x5000:
        DbgBtlHitDispPage();
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
    DbgPrintf(2, 2, "%sSYS%s", "\\c1 ", "\\c0 ");
    DbgPrintf(2, 3, "> CAMERA");
    DbgPrintf(2, 4, "> INFO");
    DbgPrintf(2, 5, "> TRACE");
    DbgPrintf(2, 6, "> HIT EFFE");
    DbgPrintf(2, 7, "> HIT DISP");
}

void DbgBtlHitDispPage(void) {
    SysState *m = &sysState;
    Pad *pad;

    if (!(m->dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        DbgBtlLineSelect(pad, 2);
        switch (m->dbgCursor) {
        case 0:
            if (pad->repeat & 0xA000) {
                dbgBtlHitDispFlags ^= DBG_BTL_HIT_FIELD;
            }
            break;
        case 1:
            if (pad->repeat & 0xA000) {
                dbgBtlHitDispFlags ^= DBG_BTL_HIT_CHAR;
            }
            break;
        case 2:
            if (pad->repeat & 0xA000) {
                dbgBtlHitDispFlags ^= DBG_BTL_HIT_ATTACK;
            }
            break;
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, "%sHIT DISP%s", "\\c1 ", "\\c0 ");
    DbgBtlPrintOnOff(2, 3, "FIELD ", dbgBtlHitDispFlags & DBG_BTL_HIT_FIELD);
    DbgBtlPrintOnOff(2, 4, "CHAR  ", dbgBtlHitDispFlags & DBG_BTL_HIT_CHAR);
    DbgBtlPrintOnOff(2, 5, "ATACK ", dbgBtlHitDispFlags & DBG_BTL_HIT_ATTACK);
}
