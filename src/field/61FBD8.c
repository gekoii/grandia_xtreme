#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/menu.h"

void FldMenuDraw(void) {
    if (fldState.mode == 1) {
        if (fldMenuWork->screen < 0xFF) {
            ((s32 (*)(void))FldMenuGuideDraw)();
        }
    }
}
