#include "common.h"
#include "protos/field.h"
#include "field/menu.h"

void FldMenuGuideDraw(void) {
    s32 s;

    s = fldMenuWork->screen;
    if (s == 9 || s == 10 || (u8)s == 12 || (u8)s == 13 || (u8)s == 14 || (u8)s == 16 || (u8)s == 11) {
        return;
    }
    TxtWinTextureBind(5);
    GphBatchDrawSprites(fldMenuWork->topQuads3);
    GphTexBind(0xEC);
    GphBatchDrawSprites(fldMenuWork->topQuads2);
    if (fldMenuWork->unk8 != 0 && fldMenuWork->unkD != 0) {
        SprBegin(0.0f, 0.0f);
        MenuButtonPadDraw(fldMenuWork->unk8, 0x12A, 0x14);
        SprFlush();
    }
}
