#include "common.h"
#include "protos/field.h"
#include "field/menu.h"
#include "field/n23.h"


FldMenuQuad *FldMenuFractionBuild(FldMenuQuad *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    FldMenuQuad *s;
    s32 x;

    s = FldMenuNumberColorBuild(arg0, arg1, 0, arg3, arg4, 0, 1.0f, 0.8f);
    x = arg3 + FldMenuNumberWidth(arg1, 0, 1.0f);
    s->v[0].a = 0;
    s->v[0].r = 0x80;
    s->v[0].g = 0x80;
    s->v[0].b = 0x80;
    s->v[0].x = x;
    s->v[0].y = arg4;
    s->v[1].x = x + 0x14;
    s->v[1].y = arg4 + 0x10;
    s->type = 1;
    s->z = 0;
    s->v[0].u = menuDigitFont.rects[13].u0 + 1;
    s->v[0].v = menuDigitFont.rects[13].v0 + 1;
    s->v[1].u = menuDigitFont.rects[13].u1;
    s->v[1].v = menuDigitFont.rects[13].v1;
    return FldMenuNumberColorBuild(s + 1, arg2, 0, x + 0x14, arg4, 0, 1.0f, 0.8f);
}
