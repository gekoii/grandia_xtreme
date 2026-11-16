#include "common.h"
#include "pad.h"
#include "sys.h"
#include "main/n02.h"
#include "protos/main.h"

extern void DbgMemDumpPage(s32, Pad *);
extern u8 dbgInfoMenuStrInfo[];
extern u8 dbgInfoMenuStrPad[];
extern u8 dbgInfoMenuStrFile[];
extern u8 dbgInfoMenuStrSound[];
extern u8 dbgInfoMenuStrPerfermance[];
extern u8 dbgInfoMenuStrProgramCall[];
extern u8 dbgInfoMenuStrMemoryBlock[];
extern u8 dbgInfoMenuStrMemoryDump[];
extern u8 dbgInfoMenuStrSaveMemoryCard[];
extern u8 dbgInfoMenuStrLoadMemoryCard[];

void DbgInfoMenu(void) {
    Pad *p;
    s32 t;
    u16 r;

    p = &padPort1;
    switch (sysState.dbgMenuFlags & 0xF0000) {
    case 0x10000:
        DbgSystemPage();
        return;
    case 0x20000:
        DbgPadPage();
        return;
    case 0x30000:
        DbgFileMenu();
        return;
    case 0x40000:
        DbgSoundMenu();
        return;
    case 0x50000:
        DbgPerfPage();
        return;
    case 0x60000:
        DbgProgramCallPage();
        return;
    case 0x70000:
        DbgMemoryPage();
        return;
    case 0x80000:
        DbgMemDumpPage(dbgMemDumpBase, p);
        return;
    case 0x90000:
        DbgMcSavePage();
        return;
    case 0xA0000:
        DbgMcLoadPage();
        return;
    }
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        r = p->repeatFast;
        if (r & 0x1000) {
            if (--sysState.dbgCursor < 0) {
                sysState.dbgCursor = 9;
            }
        } else if (r & 0x4000) {
            t = sysState.dbgCursor + 1;
            sysState.dbgCursor = t;
            if (t >= 10) {
                sysState.dbgCursor = 0;
            }
        }
        if (p->trigger & 0x20) {
            switch (sysState.dbgCursor) {
            case 0:
                sysState.dbgMenuFlags |= 0x10000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            case 1:
                sysState.dbgMenuFlags |= 0x20000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            case 2:
                sysState.dbgMenuFlags |= 0x30000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            case 3:
                sysState.dbgMenuFlags |= 0x40000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            case 4:
                sysState.dbgMenuFlags |= 0x50000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            case 5:
                sysState.dbgMenuFlags |= 0x60000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            case 6:
                sysState.dbgMenuFlags |= 0x70000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            case 7:
                sysState.dbgMenuFlags |= 0x80000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            case 8:
                sysState.dbgMenuFlags |= 0x90000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            case 9:
                sysState.dbgMenuFlags |= 0xA0000;
                sysState.dbgCursor = 0;
                sysState.dbgPage = 0;
                break;
            }
        }
    }
    DbgDrawCursor(2, sysState.dbgCursor + 2);
    DbgPrintf(3, 2, dbgInfoMenuStrInfo);
    DbgPrintf(3, 3, dbgInfoMenuStrPad);
    DbgPrintf(3, 4, dbgInfoMenuStrFile);
    DbgPrintf(3, 5, dbgInfoMenuStrSound);
    DbgPrintf(3, 6, dbgInfoMenuStrPerfermance);
    DbgPrintf(3, 7, dbgInfoMenuStrProgramCall);
    DbgPrintf(3, 8, dbgInfoMenuStrMemoryBlock);
    DbgPrintf(3, 9, dbgInfoMenuStrMemoryDump);
    DbgPrintf(3, 10, dbgInfoMenuStrSaveMemoryCard);
    DbgPrintf(3, 11, dbgInfoMenuStrLoadMemoryCard);
}
