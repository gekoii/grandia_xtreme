#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n01.h"


void TxtGlyphBufInit(TxtGlyphBuf *arg0, TxtFontImage *arg1, s16 arg2, s16 arg3) {
    s16 v;

    arg0->cellW = 0x12;
    arg0->cellH = 0x12;
    arg0->img = arg1;
    arg0->cols = arg1->w / 18;
    arg0->count = arg0->cols * (arg1->h / 18);
    arg0->first = arg2;
    v = arg3;
    if (arg3 >= arg0->count) {
        v = arg0->count - 1;
    }
    arg0->last = v;
    TxtGlyphBufReset(arg0);
}
