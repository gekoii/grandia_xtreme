#include "common.h"
#include "protos/field.h"
#include "field/menu.h"
#include "field/n23.h"



void FldMenuSaveSlotListClear(void) {
    FldMenuQuad *a;
    FldMenuQuad *b;
    FldMenuQuad *c;
    FldMenuQuad *d;

    a = FldMenuGetSprBuf(4);
    FldMenuWinSetLayer(8, 0, &fldMenuTextures[3], a, 128.0f);
    b = FldMenuGetSprBuf(5);
    FldMenuWinSetLayer(8, 1, &fldMenuTextures[6], b, 128.0f);
    c = FldMenuGetSprBuf(6);
    FldMenuWinSetLayer(8, 2, &fldMenuTextures[2], c, 128.0f);
    d = FldMenuGetSprBuf(7);
    FldMenuWinSetLayer(8, 3, &fldMenuTextures[0], d, 128.0f);
    a->type = 0;
    b->type = 0;
    c->type = 0;
    d->type = 0;
}
