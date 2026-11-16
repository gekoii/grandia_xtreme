#include "common.h"
#include "protos/field.h"
#include "field/menu.h"

void FldMenuCursorHide(void) {
    fldMenuCursor->shown = 0;
    MenuCursorHide();
}
