#include "common.h"
#include "grxsys.h"
#include "field/n24.h"
#include "protos/field.h"

s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuMcDlgFormatPrompt(s32 arg0) {
    s32 f;

    if (arg0 == 0) {
        FldMenuWinOpen(0xB, 0x140, 0xC8, 0x1F4, 0x76, 0, 3, 0x8001);
        FldMenuWinSetActive(0xB);
    }
    FldMenuWinTextClear(0xB);
    f = sysGrxsysPac->msg->tbl[11];
    FldMenuWinTextAdd(0xB, 2, f, 0x14, 0xA, 0x14, 0x12, 7, 0x1E, 0);
    FldMenuWinTextAdd(0xB, 2, f, 0xE6, 0xA, 0x14, 0x12, 8, 0x1F, 0);
    FldMenuWinTextAdd(0xB, 2, f, 0x14, 0x1E, 0x14, 0x12, 9, 0x20, 0);
    FldMenuWinTextAdd(0xB, 2, f, 0xE2, 0x1E, 0x14, 0x12, 10, fldMenuSaveCardSlot + 4, 0);
    FldMenuWinTextAdd(0xB, 2, f, 0x11E, 0x1E, 0x14, 0x12, 11, 0x21, 0);
    FldMenuWinTextAdd(0xB, 2, f, 0x14, 0x32, 0x14, 0x12, 12, 0x22, 0);
    FldMenuMcDlgOkCancel(0xB, 0xC0, 0x58);
    FldMenuCursorReset();
    if (arg0 == 2) {
        FldMenuWinClose(0xB, 0xA, 3);
    }
    return 9;
}
