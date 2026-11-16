#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"



extern u8 dbgMenuTitleFmt[];
extern u8 dbgMenuTitleColorStr[];
extern u8 dbgMenuColor0Str[];
extern u8 dbgMenuApplicationStr[];
extern u8 dbgMenuSystemStr[];
extern u8 dbgMenuEnvironmentStr[];
extern u8 dbgMenuObjectStr[];
extern u8 dbgMenuFbEffectStr[];
extern u8 dbgMenuCameraStr[];
extern u8 dbgMenuMotionStr[];
extern u8 dbgMenuScreenDivStr[];
extern u8 dbgMenuPointLightStr[];
extern u8 dbgMenuWipeStr[];
extern u8 dbgMenuFadeStr[];
extern u8 dbgMenuParticleStr[];
extern u8 dbgMenuTextureStr[];

static inline s32 upmode(s32 n) {
    s32 sh;

    sh = 0;
    while (!(n & 0xF)) {
        n >>= 4;
        sh += 4;
    }
    n &= 0xFFFFFFF0;
    return n << sh;
}

void DbgMenuUpdate(void) {
    Pad *pad;
    s32 f;
    s32 g;
    s32 t;
    s32 n;
    s32 sh;
    u16 trig;

    pad = &padPort1;
    if (padPort1.held & 2) {
        sysState.dbgMenuFlags |= 0x20000000;
    } else {
        sysState.dbgMenuFlags &= ~0x20000000;
    }
    f = sysState.dbgMenuFlags;
    if (!(f & 0x0FFFFFFF)) {
        if (pad->trigger & 0x100) {
            t = f | 0x1000000;
            sysState.dbgMenuFlags = t;
            if (sysState.program == 3) {
                sysState.dbgMenuFlags = t + 0x100000;
            }
            sysState.dbgCursor = 0;
            sysState.dbgPage = 0;
        }
        return;
    }
    trig = pad->trigger;
    if (trig & 0x100) {
        sysState.dbgMenuFlags = 0;
        f = 0;
        sysState.dbgCursor = 0;
        sysState.dbgPage = 0;
    }
    if (trig & 0x10) {
        f = sysState.dbgMenuFlags = f ^ 0x10000000;
    }
    if (f & 0x10000000) {
        return;
    }
    if (!(f & 0x20000000) && (trig & 0x40) && (f & 0x0FFFFFFF)) {
        n = upmode(f);
        sysState.dbgPage = 0;
        sysState.dbgCursor = 0;
        sysState.dbgMenuFlags = n;
    }
    g = sysState.dbgMenuFlags;
    if ((g & 0x0FFFFFFF) == 0x1000000) {
        if (!(g & 0x20000000)) {
            if (pad->repeat & 0x1000) {
                if (--sysState.dbgCursor < 0) {
                    sysState.dbgCursor = 12;
                }
            } else if (pad->repeat & 0x4000) {
                if (++sysState.dbgCursor >= 13) {
                    sysState.dbgCursor = 0;
                }
            }
            if (trig & 0x20) {
                sh = sysState.dbgCursor;
                sysState.dbgPage = 0;
                sysState.dbgCursor = 0;
                sysState.dbgMenuFlags += (sh + 1) << 20;
            }
        }
        DbgDrawCursor(2, sysState.dbgCursor + 3);
        DbgPrintf(1, 2, dbgMenuTitleFmt, dbgMenuTitleColorStr, dbgMenuColor0Str);
        DbgPrintf(3, 3, dbgMenuApplicationStr);
        DbgPrintf(3, 4, dbgMenuSystemStr);
        DbgPrintf(3, 5, dbgMenuEnvironmentStr);
        DbgPrintf(3, 6, dbgMenuObjectStr);
        DbgPrintf(3, 7, dbgMenuFbEffectStr);
        DbgPrintf(3, 8, dbgMenuCameraStr);
        DbgPrintf(3, 9, dbgMenuMotionStr);
        DbgPrintf(3, 10, dbgMenuScreenDivStr);
        DbgPrintf(3, 11, dbgMenuPointLightStr);
        DbgPrintf(3, 12, dbgMenuWipeStr);
        DbgPrintf(3, 13, dbgMenuFadeStr);
        DbgPrintf(3, 14, dbgMenuParticleStr);
        DbgPrintf(3, 15, dbgMenuTextureStr);
    } else {
        switch (g & 0xF00000) {
        case 0xC00000:
            DbgPtclMenu();
            break;
        case 0x200000:
            DbgInfoMenu();
            break;
        case 0x300000:
            DbgEnvPage();
            break;
        case 0x400000:
            DbgObjectMenu();
            break;
        case 0x500000:
            DbgFbMenu();
            break;
        case 0x600000:
            DbgCameraPage();
            break;
        case 0x700000:
            DbgMotionMenu();
            break;
        case 0x800000:
            DbgScreenDivPage();
            break;
        case 0x900000:
            DbgPointLightPage();
            break;
        case 0xA00000:
            DbgWipePage();
            break;
        case 0xB00000:
            DbgFadePage();
            break;
        case 0x100000:
            SysProgDebugMenu();
            break;
        case 0xD00000:
            DbgTexturePage();
            break;
        }
    }
}
