#include "common.h"
#include "field/menu.h"

/* One 0x1400-byte page of FldMenuWin.sprPages. */
typedef struct {
    u8 pad[0x1400];
} Page;

Page *FldMenuWinGetSprBuf(u8 arg0, u8 arg1) {
    FldMenuWin *w;

    w = &fldMenuWins[arg0];
    return &((Page *)w->sprPages)[arg1];
}
