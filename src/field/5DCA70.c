#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "sys.h"
#include "field/state.h"

extern s32 mapData;
extern char dbgFldMenuStrMonsterInfo[];
extern char dbgFldMenuStrScenario[];
extern char dbgFldMenuStrCameraScript[];
extern char dbgFldMenuStrParam[];
extern char dbgFldMenuStrParty[];
extern char dbgFldMenuStrTenant[];
extern char dbgFldMenuStrMapJump[];
extern char dbgFldMenuStrScnrFlag[];
extern char dbgFldMenuStrOcc[];
extern char dbgFldMenuStrCharaMove[];
extern char dbgFldMenuStrLine[];
extern char dbgFldMenuStrCollision[];
extern char dbgFldMenuStrSystem[];
extern char dbgFldMenuStrModel[];
extern char dbgFldMenuStrPerformance[];


void DbgFldMenu(void) {
    Pad *pad;

    switch (sysState.dbgMenuFlags & 0xF0000) {
    case 0x10000:
        DbgFldMonsterInfoMenu();
        return;
    case 0x20000:
        DbgFldScenarioMenu();
        return;
    case 0x30000:
        DbgFldCamScriptMenu();
        return;
    case 0x40000:
        DbgFldParamMenu();
        return;
    case 0x50000:
        DbgFldPartyMenu();
        return;
    case 0x60000:
        DbgFldTenantMenu(&chrTenantList);
        return;
    case 0x70000:
        DbgFldMapJumpPage(mapData);
        return;
    case 0x80000:
        DbgFldGameDebugMenu();
        return;
    case 0x90000:
        DbgFldOccMenu();
        return;
    case 0xA0000:
        DbgFldCharaMovePage();
        return;
    case 0xB0000:
        DbgFldLinesPage();
        return;
    case 0xC0000:
        DbgFldCollisionPage();
        return;
    case 0xD0000:
        DbgFldSystemMenu();
        return;
    case 0xE0000:
        DbgFldModelPage();
        return;
    case 0xF0000:
        DbgFldPerfPage();
        return;
    }
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (pad->repeatFast & 0x1000) {
            if (--sysState.dbgCursor < 0) {
                sysState.dbgCursor = 14;
            }
        } else if (pad->repeatFast & 0x4000) {
            if (++sysState.dbgCursor >= 15) {
                sysState.dbgCursor = 0;
            }
        }
        if (pad->trigger & 0x20) {
            switch (sysState.dbgCursor) {
            case 0:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0x10000;
                break;
            case 1:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0x20000;
                break;
            case 2:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0x30000;
                break;
            case 3:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0x40000;
                break;
            case 4:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0x50000;
                break;
            case 5:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0x60000;
                break;
            case 6:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0x70000;
                break;
            case 7:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0x80000;
                break;
            case 8:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0x90000;
                break;
            case 9:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0xA0000;
                break;
            case 10:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0xB0000;
                break;
            case 11:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0xC0000;
                break;
            case 12:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0xD0000;
                break;
            case 13:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0xE0000;
                break;
            case 14:
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags |= 0xF0000;
                break;
            }
        }
    }
    DbgDrawCursor(2, sysState.dbgCursor + 2);
    DbgPrintf(3, 2, dbgFldMenuStrMonsterInfo);
    DbgPrintf(3, 3, dbgFldMenuStrScenario);
    DbgPrintf(3, 4, dbgFldMenuStrCameraScript);
    DbgPrintf(3, 5, dbgFldMenuStrParam);
    DbgPrintf(3, 6, dbgFldMenuStrParty);
    DbgPrintf(3, 7, dbgFldMenuStrTenant);
    DbgPrintf(3, 8, dbgFldMenuStrMapJump);
    DbgPrintf(3, 9, dbgFldMenuStrScnrFlag);
    DbgPrintf(3, 10, dbgFldMenuStrOcc);
    DbgPrintf(3, 11, dbgFldMenuStrCharaMove);
    DbgPrintf(3, 12, dbgFldMenuStrLine);
    DbgPrintf(3, 13, dbgFldMenuStrCollision);
    DbgPrintf(3, 14, dbgFldMenuStrSystem);
    DbgPrintf(3, 15, dbgFldMenuStrModel);
    DbgPrintf(3, 16, dbgFldMenuStrPerformance);
}
