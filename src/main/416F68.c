#include "common.h"
#include "protos/main.h"
#include "main/n01.h"

extern u8 txtGlyphOverflowMsg[];

void TxtGlyphBufPut(TxtGlyphBuf *arg0, u16 arg1) {
    if (arg0->cur > arg0->last) {
        printf(txtGlyphOverflowMsg, arg1);
        return;
    }
    arg0->col++;
    if (arg0->col >= arg0->cols) {
        arg0->col = 0;
        arg0->row++;
    }
    TxtFontImagePutGlyph18(arg0->img, TxtCharToGlyph(arg1), arg0->col, arg0->row);
    arg0->cur++;
}
