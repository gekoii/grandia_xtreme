/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n28.h"
#include "battle/unit.h"
#include "pad.h"
#include "sys.h"

extern s8 dbgBtlHitAttr;
extern char dbgBtlTimestopFmt[];
extern char dbgBtlTopTitleFmt[];
extern char dbgBtlTopTitleColorStr[];
extern char dbgBtlTopColor0Str[];
extern char dbgBtlCharStr[];
extern char dbgBtlDataStr[];
extern char dbgBtlEnvStr[];
extern char dbgBtlSysStr[];
extern char dbgBtlSysSpeedFmt[];
extern char dbgBtlDmgRndStr[];
extern char dbgBtlNoDeathStr[];
extern char dbgBtlNoMpSpStr[];
extern char dbgBtlDemoModeStr[];
extern char dbgBtlDebugAiStr[];
extern char dbgBtlMgcGbxStr[];
extern char dbgBtlNoMotMvStr[];
extern char dbgBtlNoFogCtrStr[];
extern char dbgBtlAiMgcNoFmt[];
extern char dbgBtlAiMgcCmbFmt[];
extern char dbgBtlMcAiNoFmt[];
extern char dbgBtlMcAtkNoFmt[];
extern char dbgBtlTraceAttrFmt[];
extern char dbgBtlHitAttrFmt[];
extern char dbgBtlDarkFmt[];
extern char dbgBtlSePatFmt[];


void DbgBtlTopMenu(void) {
    Pad *pad;

    switch (sysState.dbgMenuFlags & 0xF0000) {
    case 0x10000:
        DbgBtlCharMenu();
        return;
    case 0x20000:
        DbgBtlDataMenu();
        return;
    case 0x30000:
        DbgBtlEnvMenu();
        return;
    case 0x40000:
        DbgBtlSysMenu();
        return;
    }
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        DbgBtlLineSelect(pad, 0x14);
        switch (sysState.dbgCursor) {
        case 4:
            btlSysSpeed += DbgEditValue(0, 0, 1, 1.0f);
            btlSysSpeed = (btlSysSpeed < 1) ? 1 : (btlSysSpeed > 16) ? 16 : btlSysSpeed;
            break;
        case 5:
            if (pad->repeat & 0xA000) {
                btlDebugCfg.flags ^= 1;
            }
            break;
        case 6:
            if (pad->repeat & 0xA000) {
                btlDebugCfg.flags ^= 2;
            }
            break;
        case 7:
            if (pad->repeat & 0xA000) {
                btlDebugCfg.flags ^= 4;
            }
            break;
        case 8:
            if (pad->repeat & 0xA000) {
                btlDebugCfg.flags ^= 0x10;
            }
            break;
        case 9:
            if (pad->repeat & 0xA000) {
                btlDebugCfg.flags ^= 0x20;
            }
            break;
        case 10:
            if (pad->repeat & 0xA000) {
                btlDebugCfg.flags ^= 0x40;
            }
            break;
        case 11:
            if (pad->repeat & 0xA000) {
                btlDebugCfg.flags ^= 0x80;
            }
            break;
        case 12:
            if (pad->repeat & 0xA000) {
                btlDebugCfg.flags ^= 0x100;
            }
            break;
        case 13:
            btlDebugCfg.mgcNo += DbgEditValue(0, 0, 3, 1.0f);
            btlDebugCfg.mgcNo = (btlDebugCfg.mgcNo < 0) ? 0 : (btlDebugCfg.mgcNo > 0x11A) ? 0x11A : btlDebugCfg.mgcNo;
            break;
        case 14:
            btlDebugCfg.mgcCmb += DbgEditValue(0, 0, 1, 1.0f);
            btlDebugCfg.mgcCmb = (btlDebugCfg.mgcCmb < 1) ? 1 : (btlDebugCfg.mgcCmb > 4) ? 4 : btlDebugCfg.mgcCmb;
            break;
        case 15:
            btlDebugCfg.aiNo += DbgEditValue(0, 0, 1, 1.0f);
            btlDebugCfg.aiNo = (btlDebugCfg.aiNo < -1) ? -1 : (btlDebugCfg.aiNo > 9) ? 9 : btlDebugCfg.aiNo;
            break;
        case 16:
            btlDebugCfg.atkNo += DbgEditValue(0, 0, 1, 1.0f);
            btlDebugCfg.atkNo = (btlDebugCfg.atkNo < -1) ? -1 : (btlDebugCfg.atkNo > 4) ? 4 : btlDebugCfg.atkNo;
            break;
        case 17:
            dbgBtlTraceAttr += DbgEditValue(0, 0, 1, 1.0f);
            dbgBtlTraceAttr = (dbgBtlTraceAttr < -1) ? -1 : (dbgBtlTraceAttr > 10) ? 10 : dbgBtlTraceAttr;
            break;
        case 18:
            dbgBtlHitAttr += DbgEditValue(0, 0, 1, 1.0f);
            dbgBtlHitAttr = (dbgBtlHitAttr < -1) ? -1 : (dbgBtlHitAttr > 8) ? 8 : dbgBtlHitAttr;
            break;
        case 19:
            btlDebugCfg.dimKind += DbgEditValue(0, 0, 1, 1.0f);
            btlDebugCfg.dimKind = (btlDebugCfg.dimKind < -1) ? -1 : (btlDebugCfg.dimKind > 3) ? 3 : btlDebugCfg.dimKind;
            break;
        case 20:
            btlDebugCfg.se += DbgEditValue(0, 0, 1, 1.0f);
            btlDebugCfg.se = (btlDebugCfg.se < -1) ? -1 : (btlDebugCfg.se > 1) ? 1 : btlDebugCfg.se;
            break;
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
            }
        }
    }
    if (sysState.dbgCursor < 13) {
        DbgDrawCursor(1, sysState.dbgCursor + 3);
    } else {
        DbgDrawCursor(0x15, sysState.dbgCursor - 10);
    }
    DbgPrintf(2, 0x13, dbgBtlTimestopFmt, btlActingUnit);
    DbgPrintf(2, 2, dbgBtlTopTitleFmt, dbgBtlTopTitleColorStr, dbgBtlTopColor0Str);
    DbgPrintf(2, 3, dbgBtlCharStr);
    DbgPrintf(2, 4, dbgBtlDataStr);
    DbgPrintf(2, 5, dbgBtlEnvStr);
    DbgPrintf(2, 6, dbgBtlSysStr);
    DbgPrintf(2, 7, dbgBtlSysSpeedFmt, btlSysSpeed);
    DbgBtlPrintOnOff(2, 8, dbgBtlDmgRndStr, (btlDebugCfg.flags ^ 1) & 1);
    DbgBtlPrintOnOff(2, 9, dbgBtlNoDeathStr, btlDebugCfg.flags & 2);
    DbgBtlPrintOnOff(2, 10, dbgBtlNoMpSpStr, btlDebugCfg.flags & 4);
    DbgBtlPrintOnOff(2, 11, dbgBtlDemoModeStr, btlDebugCfg.flags & 0x10);
    DbgBtlPrintOnOff(2, 12, dbgBtlDebugAiStr, btlDebugCfg.flags & 0x20);
    DbgBtlPrintOnOff(2, 13, dbgBtlMgcGbxStr, btlDebugCfg.flags & 0x40);
    DbgBtlPrintOnOff(2, 14, dbgBtlNoMotMvStr, btlDebugCfg.flags & 0x80);
    DbgBtlPrintOnOff(2, 15, dbgBtlNoFogCtrStr, btlDebugCfg.flags & 0x100);
    DbgPrintf(0x16, 3, dbgBtlAiMgcNoFmt, btlDebugCfg.mgcNo);
    DbgPrintf(0x16, 4, dbgBtlAiMgcCmbFmt, btlDebugCfg.mgcCmb);
    DbgPrintf(0x16, 5, dbgBtlMcAiNoFmt, btlDebugCfg.aiNo);
    DbgPrintf(0x16, 6, dbgBtlMcAtkNoFmt, btlDebugCfg.atkNo);
    DbgPrintf(0x16, 7, dbgBtlTraceAttrFmt, dbgBtlTraceAttr);
    DbgPrintf(0x16, 8, dbgBtlHitAttrFmt, dbgBtlHitAttr);
    DbgPrintf(0x16, 9, dbgBtlDarkFmt, btlDebugCfg.dimKind);
    DbgPrintf(0x16, 10, dbgBtlSePatFmt, btlDebugCfg.se);
}
