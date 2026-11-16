#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void FldMenuMcDlgOpen(void) {
    if (fldMenuMcDlgIsOpen == 0) {
        FldMenuWinOpen(0xB, 0x140, 0xC8, 0x190, 0x58, 0, 3, 0x8001);
        fldMenuMcDlgIsOpen = 1;
    }
}
