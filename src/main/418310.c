#include "common.h"
#include "main/n02.h"

void MenuFrameInit(MenuFrame *o, s16 x, s16 y) {
    o->sx = 1.0f;
    o->sy = 1.0f;
    o->angle = 0;
    o->alpha = 0;
    o->unk20 = 0;
    o->w = x;
    o->h = y;
    o->ox = 0;
    o->oy = 0;
    o->bg.w = x + 2;
    o->bg.h = y + 2;
    o->bg.originX = 1;
    o->bg.originY = 1;
    o->bg.col[2].word = 0x093E334C;
    o->bg.col[3].word = 0x7F574C;
    o->bg.col[0].word = 0x093E334C;
    o->bg.col[1].word = 0x7F574C;
}
