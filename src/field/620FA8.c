#include "common.h"
#include "protos/field.h"
#include "field/menu.h"

void FldMenuCursorReset(void) {
    fldMenuCursor->shown = 0;
    fldMenuCursor->lrLock = 0;
    MenuCursorHide();
}
