#include "common.h"
#include "field/menu.h"

void FldMenuWinSetLayerVisible(u8 arg0, s32 arg1, u8 arg2) {
    FldMenuWin *e = &fldMenuWins[arg0];
    e->layerKind[arg1] = arg2;
}
