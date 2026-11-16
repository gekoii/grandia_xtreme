#include "common.h"
#include "field/menu.h"


void FldMenuWinSetColors(u8 arg0, u32 c0, u32 c1, u32 c2, u32 c3) {
    FldMenuWin *w;

    w = &fldMenuWins[arg0];
    w->unk44 = 1;
    w->col[0][0] = c0;
    w->col[0][1] = c0 >> 8;
    w->col[0][2] = c0 >> 16;
    w->col[1][0] = c1;
    w->col[1][1] = c1 >> 8;
    w->col[1][2] = c1 >> 16;
    w->col[2][0] = c2;
    w->col[2][1] = c2 >> 8;
    w->col[2][2] = c2 >> 16;
    w->col[3][0] = c3;
    w->col[3][1] = c3 >> 8;
    w->col[3][2] = c3 >> 16;
}
