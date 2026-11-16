#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"



extern u8 dbgPtclStarterStr[];
extern u8 dbgPtclControlerStr[];
extern u8 dbgPtclEffectorStr[];
extern u8 dbgPtclGeneratorStr[];
extern u8 dbgPtclSaveFileStr[];

void DbgPtclMenu(void) {
    Pad *pad;

    switch (sysState.dbgMenuFlags & 0xF0000) {
    case 0x10000:
        DbgPtclStarterPage();
        return;
    case 0x20000:
        DbgPtclCtrlPage();
        return;
    case 0x30000:
        DbgPtclEffectorPage();
        return;
    case 0x40000:
        DbgPtclGenPage();
        return;
    }
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (pad->repeat & 0x1000) {
            if (--sysState.dbgCursor < 0) {
                sysState.dbgCursor = 4;
            }
        } else if (pad->repeat & 0x4000) {
            if (++sysState.dbgCursor >= 5) {
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
                DbgPtclSaveFile();
                break;
            }
        }
    }
    DbgDrawCursor(2, sysState.dbgCursor + 2);
    DbgPrintf(3, 2, dbgPtclStarterStr);
    DbgPrintf(3, 3, dbgPtclControlerStr);
    DbgPrintf(3, 4, dbgPtclEffectorStr);
    DbgPrintf(3, 5, dbgPtclGeneratorStr);
    DbgPrintf(3, 6, dbgPtclSaveFileStr);
}
