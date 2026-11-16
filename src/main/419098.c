#include "common.h"
#include "protos/main.h"
#include "main/n02.h"

void MenuHelpOpen(s32 a0, s32 a1, void *arg2, f32 x, f32 y) {
    MenuHelp *p;
    s8 arg0 = a0;
    s8 arg1 = a1;

    p = &menuHelp;
    if (arg0 != 2 && arg2 == NULL) {
        MenuHelpClose();
        return;
    }
    x += 0.0f;
    y += 16.0f;
    p->frame.bg.col[2].word = 0x093E334C;
    p->frame.bg.col[3].word = 0x7F574C;
    p->frame.bg.col[0].word = 0x093E334C;
    p->frame.bg.col[1].word = 0x7F574C;
    switch (arg0) {
    case 0:
        p->w = 0x18C;
        p->h = 0x66;
        p->frame.bg.col[3].b[0] = 0x78;
        p->frame.bg.col[1].b[0] = 0x78;
        p->frame.bg.col[2].b[0] = 0x78;
        p->frame.bg.col[0].b[0] = 0x78;
        p->frame.bg.col[0].b[3] = p->frame.bg.col[2].b[3] = (u8)(p->frame.bg.col[0].b[3] * 0.85f);
        p->frame.bg.col[1].b[3] = p->frame.bg.col[3].b[3] = (u8)(p->frame.bg.col[1].b[3] * 0.85f);
        p->frame.bg.col[0].b[2] = p->frame.bg.col[2].b[2] = (u8)(p->frame.bg.col[0].b[2] * 0.85f);
        p->frame.bg.col[1].b[2] = p->frame.bg.col[3].b[2] = (u8)(p->frame.bg.col[1].b[2] * 0.85f);
        p->frame.bg.col[0].b[1] = p->frame.bg.col[2].b[1] = (u8)(p->frame.bg.col[0].b[1] * 0.85f);
        p->frame.bg.col[1].b[1] = p->frame.bg.col[3].b[1] = (u8)(p->frame.bg.col[1].b[1] * 0.85f);
        break;
    case 2:
        p->w = 0x196;
        p->h = 0x36;
        break;
    default:
        p->w = 0xCE;
        p->h = 0x36;
        break;
    }
    p->toX = x + p->w / 2;
    p->toY = y + p->h / 2;
    if ((p->state & 0xF0) <= 0x10) {
        p->openFrame = menuHelpFrame;
        p->state = 0x10;
        p->style = arg0;
        p->kind = arg1;
        p->data = arg2;
        p->nextStyle = 0x7F;
        p->slideTimer = 1;
        return;
    }
    p->state = 0x30;
    p->nextKind = arg1;
    p->nextData = arg2;
    p->nextStyle = arg0;
    if (arg0 != p->style) {
        p->slideTimer = 0;
        return;
    }
    p->slideTimer = 8;
    p->fromX = p->frame.x;
    p->fromY = p->frame.y;
}
