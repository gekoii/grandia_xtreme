#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void FldMenuMcDlgClose(void) {
    FldMenuWinClose(0xB, 0xA, 3);
    fldMenuMcDlgIsOpen = 0;
}
