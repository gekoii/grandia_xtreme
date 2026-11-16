#include "common.h"
#include "pad.h"
#include "sys.h"
#include "protos/main.h"



extern u8 dbgObjMenuStrPartsInfo[];
extern u8 dbgObjMenuStrAttrib[];

void DbgObjectMenu(void) {
    s32 t;
    Pad *p;
    u16 r;

    switch (sysState.dbgMenuFlags & 0xF0000) {
    case 0x10000:
        DbgObjectPage();
        return;
    case 0x20000:
        DbgObjAtrMenu();
        return;
    }
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        p = &padPort1;
        r = p->repeatFast;
        if (r & 0x1000) {
            if (--sysState.dbgCursor < 0) {
                sysState.dbgCursor = 1;
            }
        } else if (r & 0x4000) {
            t = sysState.dbgCursor + 1;
            sysState.dbgCursor = t;
            if (t >= 2) {
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
            }
        }
    }
    DbgDrawCursor(2, sysState.dbgCursor + 2);
    DbgPrintf(3, 2, dbgObjMenuStrPartsInfo);
    DbgPrintf(3, 3, dbgObjMenuStrAttrib);
}
