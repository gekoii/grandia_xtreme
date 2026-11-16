#include "common.h"
#include "protos/main.h"
#include "grxsys.h"
#include "sys.h"
#include "main/n01.h"


void TxtFont18Init(TxtGlyphBuf *arg0, TxtFontImage *arg1, s32 arg2, s32 arg3) {
    s32 r;

    r = MemCalloc16A(TxtFontImageCalcSize(arg1, 0x10, 0x10, arg2, arg3));
    TxtFontImageSetup(arg1, 0x10, 0x10, arg2, arg3, sysGrxsysPac->fontTex, 0, r);
    arg2 = arg1->w;
    arg3 = arg1->h;
    arg0->cellW = 0x12;
    arg0->cellH = 0x12;
    arg0->img = arg1;
    arg0->cols = arg2 / 18;
    arg0->count = arg0->cols * (arg3 / 18);
    arg0->last = arg0->count - 1;
    arg0->first = 0;
    TxtGlyphBufReset(arg0);
}
