#include "common.h"
#include "protos/field.h"
#include "field/menu.h"
#include "field/n23.h"

void MemSet(void *, s32, s32);

FldMenuQuad *FldMenuPercentBuild(FldMenuQuad *s, s32 num, s32 unused, s32 x, s32 y, u8 col, f32 sx, f32 sy) {
    u8 buf[8];
    s32 i;

    MemSet(buf, 0, 8);
    i = UtilIntToDigits(num, buf);
    for (; i < 8; i++) {
        s->type = 1;
        if (col != 0) {
            s->v[0].r = 0x80;
            s->v[0].g = 0x10;
            s->v[0].b = 0x10;
            s->v[0].a = 0x80;
        } else {
            s->v[0].r = 0x80;
            s->v[0].g = 0x80;
            s->v[0].b = 0x80;
            s->v[0].a = 0x80;
        }
        s->v[0].x = x;
        s->v[1].x = x + (s32)(fldMenuDigitFont2.w[buf[i]] * sx);
        s->v[0].y = y;
        s->v[1].y = y + (s32)(fldMenuDigitFont2.rects[buf[i]].h * sy);
        s->z = 0;
        s->v[0].u = fldMenuDigitFont2.rects[buf[i]].u0 + 1;
        s->v[0].v = fldMenuDigitFont2.rects[buf[i]].v0 + 1;
        s->v[1].u = fldMenuDigitFont2.rects[buf[i]].u1;
        s->v[1].v = fldMenuDigitFont2.rects[buf[i]].v1;
        x += (s32)(fldMenuDigitFont2.w[buf[i]] * sx);
        s++;
    }
    s->v[0].r = 0x80;
    s->v[0].g = 0x80;
    s->v[0].b = 0x80;
    s->v[0].a = 0x80;
    s->type = 1;
    s->v[0].x = x;
    s->v[1].x = x + (s32)(fldMenuDigitFont2.w[11] * sx);
    s->v[0].y = y;
    s->v[1].y = y + (s32)(fldMenuDigitFont2.rects[11].h * sy);
    s->z = 0;
    s->v[0].u = fldMenuDigitFont2.rects[11].u0 + 1;
    s->v[0].v = fldMenuDigitFont2.rects[11].v0 + 1;
    s->v[1].u = fldMenuDigitFont2.rects[11].u1;
    s->v[1].v = fldMenuDigitFont2.rects[11].v1;
    s++;
    s->type = 0;
    return s;
}
