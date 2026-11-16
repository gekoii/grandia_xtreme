#include "common.h"
#include "protos/field.h"
#include "field/menu.h"
#include "field/n23.h"

void MemSet(void *, s32, s32);

FldMenuQuad *FldMenuNumberColorBuild(FldMenuQuad *s, s32 num, u8 align, s32 x, s32 y, u8 col, f32 sx, f32 sy) {
    u8 buf[8];
    s32 i;
    s32 n;
    s32 w;

    MemSet(buf, 0, 8);
    n = UtilIntToDigits(num, buf);
    if (align == 1) {
        w = 0;
        for (i = n; i < 8; i++) {
            w += (s32)(menuDigitFont.w[buf[i]] * sx);
        }
        x -= w;
    }
    for (i = n; i < 8; i++) {
        s->type = 1;
        switch (col) {
        case 0:
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
            s->v[0].a = 0x80;
            break;
        case 1:
            s->v[0].r = 0x40;
            s->v[0].g = 0x78;
            s->v[0].b = 0x58;
            s->v[0].a = 0x80;
            break;
        case 2:
            s->v[0].r = 0x78;
            s->v[0].g = 0x74;
            s->v[0].b = 0x30;
            s->v[0].a = 0x80;
            break;
        case 3:
            s->v[0].r = 0x50;
            s->v[0].a = 0x80;
            s->v[0].g = 0;
            s->v[0].b = 0;
            break;
        }
        s->v[0].x = x;
        s->v[1].x = x + (s32)(menuDigitFont.w[buf[i]] * sx);
        s->v[0].y = y;
        s->v[1].y = y + (s32)(menuDigitFont.rects[buf[i]].h * sy);
        s->z = 0;
        s->v[0].u = menuDigitFont.rects[buf[i]].u0 + 1;
        s->v[0].v = menuDigitFont.rects[buf[i]].v0 + 1;
        s->v[1].u = menuDigitFont.rects[buf[i]].u1;
        s->v[1].v = menuDigitFont.rects[buf[i]].v1;
        x += (s32)(menuDigitFont.w[buf[i]] * sx);
        s++;
    }
    s->type = 0;
    return s;
}
