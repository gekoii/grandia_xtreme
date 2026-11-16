#include "common.h"
#include "grxsys.h"
#include "field/n24.h"
#include "main/n03.h"
#include "protos/field.h"

s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuMcDlgSlotSelect(s32 arg0, s32 arg1) {
    s32 f;
    McPortState *s;

    fldMenuSaveCardSlot = arg1;
    if (arg0 == 0) {
        FldMenuWinOpen(0xB, 0x140, 0xC8, 0x14E, 0x60, 0, 3, 0x8001);
        FldMenuWinSetActive(0xB);
    }
    FldMenuWinClose(9, 0xA, 2);
    FldMenuWinClose(8, 0xA, 0);
    FldMenuWinClose(0xA, 0xA, 3);
    FldMenuWinTextClear(0xB);
    f = sysGrxsysPac->msg->tbl[11];
    FldMenuWinTextAdd(0xB, 2, f, 0x14, 0xF, 0x14, 0x12, 3, 0x12, 0);
    s = McGetPortState(0);
    if (s->state == 2 && s->type == s->state) {
        FldMenuWinTextAdd(0xB, 2, f, 0x30, 0x2C, 0x14, 0x12, 0x12, 3, 0);
        FldMenuWinTextAdd(0xB, 2, f, 0xE2, 0x2C, 0x14, 0x12, 4, 4, 0);
    } else {
        FldMenuWinTextAdd(0xB, 2, f, 0x30, 0x2C, 0x14, 0x12, 0x12, 3, 0xA);
        FldMenuWinTextAdd(0xB, 2, f, 0xE2, 0x2C, 0x14, 0x12, 4, 4, 0xA);
    }
    s = McGetPortState(1);
    if (s->state == 2 && s->type == s->state) {
        FldMenuWinTextAdd(0xB, 2, f, 0x30, 0x40, 0x14, 0x12, 0x12, 3, 0);
        FldMenuWinTextAdd(0xB, 2, f, 0xE2, 0x40, 0x14, 0x12, 5, 5, 0);
    } else {
        FldMenuWinTextAdd(0xB, 2, f, 0x30, 0x40, 0x14, 0x12, 0x12, 3, 0xA);
        FldMenuWinTextAdd(0xB, 2, f, 0xE2, 0x40, 0x14, 0x12, 5, 5, 0xA);
    }
    FldMenuCursorSetAt(0xB, 36.0f, (f32)(arg1 * 20) + 54.0f);
    if (arg0 == 2) {
        FldMenuMcDlgClose();
    }
    return 9;
}
