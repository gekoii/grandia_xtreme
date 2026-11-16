#include "common.h"
#include "protos/field.h"
#include "field/menu.h"


void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void FldMenuWinInit(void) {
    FldMenuCursor *p;

    fldMenuWins = FldMemAlloc(0x1812C0);
    fldMenuCursor = FldMemAlloc(0x80);
    MemSet(fldMenuWins, 0, 0x1812C0);
    MemSet(fldMenuCursor, 0, 0x80);
    MenuWidgetsInit();
    MenuHelpInit();
    p = fldMenuCursor;
    p->colorKeys[0][0] = 173.0f;
    p->colorKeys[0][1] = 149.0f;
    p->colorKeys[0][2] = 96.0f;
    p->colorKeys[0][3] = 0.0f;
    p->colorKeys[1][0] = 145.0f;
    p->colorKeys[1][1] = 116.0f;
    p->colorKeys[1][2] = 55.0f;
    p->colorKeys[1][3] = 0.0f;
    p->colorKeys[2][0] = 240.0f;
    p->colorKeys[2][1] = 149.0f;
    p->colorKeys[2][2] = 96.0f;
    p->colorKeys[2][3] = 0.0f;
    p->colorKeys[3][0] = 240.0f;
    p->colorKeys[3][1] = 116.0f;
    p->colorKeys[3][2] = 55.0f;
    p->colorKeys[3][3] = 0.0f;
    p->t = 0;
}
