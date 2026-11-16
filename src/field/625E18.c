#include "common.h"
#include "protos/field.h"
#include "field/menu.h"
#include "field/n23.h"

void MemSet(void *, s32, s32);

FldMenuQuad *FldMenuNumberTintBuild(FldMenuQuad *s, s32 num, s32 unused, s32 x, s32 y, u8 alpha, u32 mode, f32 sx, f32 sy) {
    u8 buf[8];
    s32 i;
    s32 n;
    u8 r, g, b;

    MemSet(buf, 0, 8);
    n = UtilIntToDigits(num, buf);
    switch (mode) {
    default:
    case 0:
        r = 0x80;
        g = 0x80;
        b = 0x80;
        break;
    case 1:
        r = 0x80;
        g = 0x20;
        b = 0x20;
        break;
    case 2:
        r = 0x40;
        g = 0x40;
        b = 0x80;
        break;
    }
    for (i = n; i < 8; i++) {
        s->v[0].r = r;
        s->v[0].g = g;
        s->v[0].b = b;
        s->v[0].a = alpha;
        s->type = 1;
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
