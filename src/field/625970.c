#include "common.h"
#include "protos/field.h"
#include "field/menu.h"
#include "field/n23.h"

void MemSet(void *, s32, s32);

FldMenuQuad *FldMenuPlayTimeBuild(FldMenuQuad *s, s32 frames, s32 unused, s32 x, s32 y, u8 alpha, f32 sx, f32 sy) {
    u8 buf[8];
    s32 i;
    s32 n;
    s32 w;
    s32 k;
    s32 h;
    s32 m;
    s32 t;
    s32 base;
    s32 d;
    f32 fx;

    t = frames / 60;
    h = t / 3600;
    t -= h * 3600;
    d = h;
    if (d < 1000) {
        m = t / 60;
    } else {
        d = 999;
        m = 59;
    }
    MemSet(buf, 0, 8);
    n = UtilIntToDigits(d, buf);
    w = 0;
    for (i = n; i < 8; i++) {
        w += (s32)(menuDigitFont.w[buf[i]] * sx);
    }
    fx = x;
    base = (s32)(fx + sx * 36.0f - w);
    k = 0;
    for (i = n; i < 8; i++) {
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = alpha;
        s->type = 1;
        s->v[0].x = base + k;
        s->v[1].x = base + k + (s32)(menuDigitFont.w[buf[i]] * sx);
        s->v[0].y = y;
        s->v[1].y = y + (s32)(menuDigitFont.rects[buf[i]].h * sy);
        s->z = 0;
        s->v[0].u = menuDigitFont.rects[buf[i]].u0 + 1;
        s->v[0].v = menuDigitFont.rects[buf[i]].v0 + 1;
        s->v[1].u = menuDigitFont.rects[buf[i]].u1;
        s->v[1].v = menuDigitFont.rects[buf[i]].v1;
        k += (s32)(menuDigitFont.w[buf[i]] * sx);
        s++;
    }
    MemSet(buf, 0, 8);
    n = UtilIntToDigits(m, buf);
    w = 0;
    for (i = n; i < 8; i++) {
        w += (s32)(menuDigitFont.w[buf[i]] * sx);
    }
    base = (s32)(fx + sx * 36.0f + sx * 12.0f);
    base = (s32)(base + sx * 24.0f - w);
    k = 0;
    for (i = n; i < 8; i++) {
        s->v[0].r = 0x80;
        s->v[0].g = 0x80;
        s->v[0].b = 0x80;
        s->v[0].a = alpha;
        s->type = 1;
        s->v[0].x = base + k;
        s->v[1].x = base + k + (s32)(menuDigitFont.w[buf[i]] * sx);
        s->v[0].y = y;
        s->v[1].y = y + (s32)(menuDigitFont.rects[buf[i]].h * sy);
        s->z = 0;
        s->v[0].u = menuDigitFont.rects[buf[i]].u0 + 1;
        s->v[0].v = menuDigitFont.rects[buf[i]].v0 + 1;
        s->v[1].u = menuDigitFont.rects[buf[i]].u1;
        s->v[1].v = menuDigitFont.rects[buf[i]].v1;
        k += (s32)(menuDigitFont.w[buf[i]] * sx);
        s++;
    }
    s->type = 0;
    return s;
}
