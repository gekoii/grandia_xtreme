#include "common.h"
#include "field/menu.h"

void FldMenuWinSetLayer(u8 arg0, s32 arg1, GsTexPacket *arg2, FldMenuQuad *arg3, f32 arg4) {
    FldMenuWin *e = &fldMenuWins[arg0];
    e->layerTex[arg1] = (s32)arg2;
    e->layerQuad[arg1] = arg3;
    e->layerVal[arg1] = arg4;
}
