#include "common.h"
#include "field/n23.h"
#include "protos/field.h"

typedef struct {
    s32 flag;
    s32 pad4;
    s32 pad8;
    s32 unkC;
    u8 r, g, b, a;
    s16 u0, v0;
    s16 x0, y0;
    s32 pad1C;
    s16 u1, v1;
    s16 x1, y1;
    u8 pad28[0x18];
} Spr;


void MemSet(void *, s32, s32);

Spr *FldMenuTaggedNumberBuild(Spr *s, s32 num, s32 unused, s32 x, s32 y, u8 col, u8 mode, f32 sx, f32 sy) {
    u8 buf[8];
    s32 i;
    s32 n;
    f32 fy;

    MemSet(buf, 0, 8);
    n = UtilIntToDigits(num, buf);
    s->flag = 1;
    switch (mode) {
    case 0:
        s->r = 0x80;
        s->g = 0x80;
        s->b = 0x80;
        s->a = col;
        break;
    case 8:
        s->r = 0x50;
        s->g = 0x50;
        s->b = 0x30;
        s->a = col;
        break;
    case 10:
        s->r = 0x50;
        s->g = 0x50;
        s->b = 0x50;
        s->a = col;
        break;
    }
    s->x0 = x;
    fy = y + sy * 8.0f;
    s->y0 = fy;
    s->x1 = x + (s32)(sx * 20.0f);
    s->y1 = fy + ((s32)(sy * 20.0f) - sy * 4.0f);
    s->unkC = 0;
    s->u0 = menuDigitFont.rects[14].u0 + 1;
    s->v0 = menuDigitFont.rects[14].v0 + 1;
    s->u1 = menuDigitFont.rects[14].u1;
    s->v1 = menuDigitFont.rects[14].v1;
    x += (s32)(sx * 20.0f);
    s++;
    for (i = n; i < 8; i++) {
        s->flag = 1;
        switch (mode) {
        case 0:
            s->r = 0x80;
            s->g = 0x80;
            s->b = 0x80;
            s->a = col;
            break;
        case 8:
            s->r = 0x50;
            s->g = 0x50;
            s->b = 0x30;
            s->a = col;
            break;
        case 10:
            s->r = 0x50;
            s->g = 0x50;
            s->b = 0x50;
            s->a = col;
            break;
        }
        s->x0 = x;
        s->x1 = x + (s32)(menuDigitFont.w[buf[i]] * sx);
        s->y0 = y;
        s->y1 = y + (s32)(menuDigitFont.rects[buf[i]].h * sy);
        s->unkC = 0;
        s->u0 = menuDigitFont.rects[buf[i]].u0 + 1;
        s->v0 = menuDigitFont.rects[buf[i]].v0 + 1;
        s->u1 = menuDigitFont.rects[buf[i]].u1;
        s->v1 = menuDigitFont.rects[buf[i]].v1;
        x += (s32)(menuDigitFont.w[buf[i]] * sx);
        s++;
    }
    s->flag = 0;
    return s;
}
