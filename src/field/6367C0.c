#include "common.h"
#include "grxsys.h"
#include "field/n24.h"
#include "protos/field.h"

s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuMcDlgIncompatible(s32 arg0) {
    s32 f;

    if (arg0 == 0) {
        FldMenuWinOpen(0xB, 0x140, 0xC8, 0x190, 0x58, 0, 3, 0x8001);
        FldMenuWinSetActive(0xB);
    }
    FldMenuWinTextClear(0xB);
    f = sysGrxsysPac->msg->tbl[11];
    FldMenuWinTextAdd(0xB, 2, f, 0x14, 0xA, 0x14, 0x12, 6, 0x35, 0);
    FldMenuWinTextAdd(0xB, 2, f, 0xD3, 0xA, 0x14, 0x12, 7, 0x36, 0);
    FldMenuCursorReset();
    if (arg0 == 2) {
        FldMenuWinClose(0xB, 0xA, 3);
    }
    return 9;
}
