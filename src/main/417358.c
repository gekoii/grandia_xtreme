/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "main/n02.h"


void MenuTextListDraw(TxtObj **arg0) {
    TxtObj *dst;
    TxtObj *src;
    TxtObj **pp;
    f32 save;
    s16 n;
    u8 a, b, c;
    void *t;
    SprRectListEntry *p4;
    SprRect *p8;

    n = 0;
    pp = arg0 + 1;
    dst = arg0[0];
    save = dst->alignX;
    a = menuTextAddR;
    b = menuTextAddG;
    c = menuTextAddB;
    while (*pp != NULL) {
        src = *pp;
        t = src->glyphs;
        p4 = src->sprites;
        p8 = src->rects;
        dst->sprites = p4;
        dst->glyphs = t;
        dst->rects = p8;
        dst->style = src->style;
        dst->glyphFirst = (*pp)->glyphFirst;
        dst->glyphLast = (*pp)->glyphLast;
        dst->alignX = save - (f32)n / (f32)dst->width;
        dst->spacing = (*pp)->spacing;
        n = n + ((f32)(*pp)->width / (*pp)->scale) * dst->scale;
        pp++;
        menuTextAddR = a;
        menuTextAddG = b;
        menuTextAddB = c;
        MenuTextDraw(dst);
    }
    dst->alignX = save;
}
