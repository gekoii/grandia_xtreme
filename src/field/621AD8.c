#include "common.h"
#include "field/menu.h"


void FldMenuWinSetPortrait(u8 arg0, u16 arg1, u16 arg2, s32 arg3, s32 arg4) {
    FldMenuWin *e = &fldMenuWins[arg0];
    e->portraitCode = arg1;
    e->unk16 = arg2;
    e->x = arg3;
    e->y = arg4;
    e->w = 0x60;
    e->h = 0x60;
}
