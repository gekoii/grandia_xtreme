#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n07.h"

void MemMove32(void *, void *, s32);

void TxtFontImageSetup(TxtFontImage *p, s32 a1, s32 a2, s32 a3, s32 t0, s32 t1, s32 pal, s32 t3) {
    if (p == NULL || t3 == 0) {
        return;
    }
    GsTexInfoInit(p->texDim, p->pixels, p->clut, a3, t0);
    p->cellW = a1;
    p->pixels = (u8 *)t3;
    p->w = p->texDim[0];
    p->cellH = a2;
    t0 = p->texDim[1];
    p->h = t0;
    p->glyphMetricsSrc = 0;
    p->glyphSrc = (u8 *)t1;
    p->pixelSize = p->w / 2 * p->h;
    p->unk49C = t3;
    GsTexLoadPacket(&p->texPacket, p->texDim, 0xD6000, 0xD2400, 0, 1, 1, 0, 0, 0, 0, 0);
    MemMove32(p->clut, txtFontAlphaClut[pal], 16);
}
