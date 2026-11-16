#include "common.h"
#include "protos/field.h"


void FldMenuItemListWinOpen(u8 arg0, u8 arg1) {
    FldMenuWinOpen(arg0, 0x140, 0xE6, 0x200, 0xBA, 0, arg1, 0x88010);
    FldMenuWinSetActive(arg0);
    FldMenuWinLrArrowsSet(arg0, 1, 12.0f);
}
