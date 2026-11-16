/* rodata: 0x006E29F0-0x006E2AC0 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"
#include "pad.h"
#include "sys.h"

static const char save[] = "save";


void DbgBtlEnvMenu(void) {
    SysState *m;
    Pad *pad;

    switch (sysState.dbgMenuFlags & 0xF000) {
    case 0x1000:
        DbgEnvLightPage();
        return;
    case 0x2000:
        DbgBtlEncountPage();
        return;
    case 0x3000:
        DbgEnvFieldPage();
        return;
    case 0x4000:
        DbgEnvShadowPage();
        return;
    case 0x5000:
        DbgEnvLensFlarePage();
        return;
    case 0x6000:
        DbgEnvContrastPage();
        return;
    }
    m = &sysState;
    if (!(m->dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        DbgBtlLineSelect(pad, 5);
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
            case 5:
                sysState.dbgPage = 0; sysState.dbgCursor = 0; sysState.dbgMenuFlags |= 0x6000;
                break;
            }
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, "%sENV%s", "\\c1 ", "\\c0 ");
    DbgPrintf(2, 3, "> LIGHT");
    DbgPrintf(2, 4, "> ENCOUNT");
    DbgPrintf(2, 5, "> FIELD");
    DbgPrintf(2, 6, "> SHADOW");
    DbgPrintf(2, 7, "> LENSFLARE");
    DbgPrintf(2, 8, "> CONTRAST");
}

void DbgBtlEncountPage(void) {
    s32 i;
    s32 id;
    BtlUnit *u;

    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, "%sENCOUNT%s", "\\c1 ", "\\c0 ");
    DbgPrintf(2, 3, "PC");
    DbgPrintf(2, 7, "MC");
    for (i = 0; i < 16; i++) {
        u = &btlUnits[i];
        id = u->def ? u->def->slot : -1;
        DbgPrintf(5, i + 3, "0x%.4X (%d) data:%d, id=%.3X", btlUnitIds.ids[i], btlUnitIds.flags[i],
                      id, u->params->id & 0xFFF);
    }
    for (i = 0; i < 4; i++) {
        DbgPrintf(0x28, i + 7, "%.3X", btlHandoff.enemyKind[i] & 0xFFF);
    }
}
