#include "common.h"
#include "field/menu.h"

s32 FldMenuWinGetQuadBuf(u8 arg0) {
    return (s32)fldMenuWins[arg0].quads;
}

void FldMenuCursorGetPos(f32 *arg0, f32 *arg1) {
    *arg0 = fldMenuCursor->x;
    *arg1 = fldMenuCursor->y;
}
