#include "common.h"
#include "protos/field.h"
#include "field/menu.h"

void FldMenuLrArrowsSetAt(u8 arg0, f32 x, f32 y, f32 z) {
    if (!fldMenuCursor->lrLock) {
        MenuLrArrowsSet(x, y, z);
    }
    fldMenuCursor->lrLock = arg0;
}
