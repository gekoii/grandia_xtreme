#include "common.h"
#include "protos/main.h"
#include "main/n01.h"

void TxtObjSetStringList(TxtObj *arg0, TxtGlyphBuf *arg1, s32 *arg2) {
    arg0->glyphFirst = arg1->cur;
    if (arg2 != NULL && *arg2 != 0) {
        arg0->width = TxtStringListWidth(arg1, arg2, arg0->spacing) * arg0->scale;
    } else {
        arg0->width = 0;
    }
    arg0->glyphs = arg1;
    arg0->sprites = 0;
    arg0->glyphLast = arg1->cur - 1;
}
