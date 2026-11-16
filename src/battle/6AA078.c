#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "pad.h"
#include "sys.h"

extern char dbgInfoStrTitle[];
extern char dbgInfoStrColHi[];
extern char dbgInfoStrColLo[];
extern char dbgInfoStrHp[];
extern char dbgInfoStrIp[];
extern char dbgInfoStrDmg[];
extern char dbgInfoStrCmr[];
extern char dbgInfoStrRepo[];
extern char dbgInfoStrIpX[];
extern char dbgInfoStrIpY[];
extern char dbgInfoStrSx[];
extern char dbgInfoStrSy[];
extern char dbgInfoStrAng[];
extern char dbgInfoStrRate[];
extern char dbgInfoStrDelta[];
extern char dbgInfoStrCng[];


void DbgBtlInfoPage(void) {
    Pad *pad;

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        DbgBtlLineSelect(pad, 0xD);
        switch (sysState.dbgCursor) {
        case 0:
            if (pad->repeat & 0xA000) btlHudHideFlags ^= 2;
            break;
        case 1:
            if (pad->repeat & 0xA000) btlHudHideFlags ^= 1;
            break;
        case 2:
            if (pad->repeat & 0xA000) btlHudHideFlags ^= 4;
            break;
        case 3:
            if (pad->repeat & 0xA000) btlHudHideFlags ^= 8;
            break;
        case 4:
            if (pad->repeat & 0xA000) btlHudHideFlags ^= 0x10;
            break;
        case 5:
            ipDialX += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 6:
            ipDialY += DbgEditValue(0, 0, 3, 1.0f);
            break;
        case 7:
            ipDialScaleX += DbgEditValue(2, -1, 1, 0.1f);
            break;
        case 8:
            ipDialScaleY += DbgEditValue(2, -1, 1, 0.1f);
            break;
        case 9:
            ipDialAng += DbgEditValue(1, 0, 4, 256.0f);
            break;
        case 10:
            ipTransRate += DbgEditValue(2, -3, 1, 0.01f);
            break;
        case 11:
            ipTransDelta += DbgEditValue(2, -3, 1, 0.01f);
            break;
        case 12:
            ipTransDelay += DbgEditValue(0, 0, 2, 1.0f);
            break;
        case 13:
            ipTransNextDelta += DbgEditValue(2, -3, 1, 0.01f);
            break;
        }
    }
    DbgDrawCursor(1, sysState.dbgCursor + 3);
    DbgPrintf(2, 2, dbgInfoStrTitle, dbgInfoStrColHi, dbgInfoStrColLo);
    DbgBtlPrintOnOff(2, 3, dbgInfoStrHp, ((btlHudHideFlags >> 1) ^ 1) & 1);
    DbgBtlPrintOnOff(2, 4, dbgInfoStrIp, (btlHudHideFlags ^ 1) & 1);
    DbgBtlPrintOnOff(2, 5, dbgInfoStrDmg, ((btlHudHideFlags >> 2) ^ 1) & 1);
    DbgBtlPrintOnOff(2, 6, dbgInfoStrCmr, ((btlHudHideFlags >> 3) ^ 1) & 1);
    DbgBtlPrintOnOff(2, 7, dbgInfoStrRepo, ((btlHudHideFlags >> 4) ^ 1) & 1);
    DbgPrintf(2, 8, dbgInfoStrIpX, ipDialX);
    DbgPrintf(2, 9, dbgInfoStrIpY, ipDialY);
    DbgPrintf(2, 10, dbgInfoStrSx, ipDialScaleX);
    DbgPrintf(2, 11, dbgInfoStrSy, ipDialScaleY);
    DbgPrintf(2, 12, dbgInfoStrAng, (u16)ipDialAng);
    DbgPrintf(2, 13, dbgInfoStrRate, ipTransRate);
    DbgPrintf(2, 14, dbgInfoStrDelta, ipTransDelta);
    DbgPrintf(2, 15, dbgInfoStrCng, ipTransDelay);
    DbgPrintf(2, 16, dbgInfoStrDelta, ipTransNextDelta);
}
