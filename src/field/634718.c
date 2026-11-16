#include "common.h"

s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuMcDlgDoNotRemove(u8 arg0, u8 arg1, s32 font, s32 x, s32 y, s32 n, s32 id) {
    FldMenuWinTextAdd(arg0, arg1, font, x, y, 0x14, 0x12, n++, 0x15, 0);
    FldMenuWinTextAdd(arg0, arg1, font, x, y + 0x14, 0x14, 0x12, n++, 0x16, 0);
    FldMenuWinTextAdd(arg0, arg1, font, x, y + 0x28, 0x14, 0x12, n++, 0x17, 0);
    FldMenuWinTextAdd(arg0, arg1, font, x + 0xED, y + 0x28, 0x14, 0x12, n++, id + 4, 0);
    FldMenuWinTextAdd(arg0, arg1, font, x + 0x129, y + 0x28, 0x14, 0x12, n + 0xD, 0x18, 0);
    return n;
}
