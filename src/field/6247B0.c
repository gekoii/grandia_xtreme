#include "common.h"
#include "protos/field.h"
#include "field/menu.h"
#include "field/n23.h"


FldMenuQuad *FldMenuHpFractionBuild(FldMenuQuad *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 c1;
    s32 c2;
    f32 sx;
    f32 sy;

    sx = 1.0f;
    sy = 0.8f;
    if (arg1 == arg2) {
        c1 = 1;
        c2 = 1;
    } else {
        c2 = 0;
        if (arg2 >= arg1 * 4) {
            c1 = 3;
        } else if (arg2 < arg1 * 2) {
            c1 = 0;
        } else {
            c1 = 2;
        }
    }
    arg0 = FldMenuNumberColorBuild(arg0, arg1, 0, arg3, arg4, c1, sx, sy);
    arg3 += FldMenuNumberWidth(arg1, 0, sx);
    arg0->v[0].r = 0x80;
    arg0->v[0].g = 0x80;
    arg0->v[0].b = 0x80;
    arg0->v[0].a = 0;
    arg0->v[0].x = arg3;
    arg0->v[0].y = arg4;
    arg0->v[1].x = arg3 + (s32)(sx * 20.0f);
    arg0->v[1].y = arg4 + (s32)(sy * 20.0f);
    arg0->type = 1;
    arg0->z = 0;
    arg0->v[0].u = menuDigitFont.rects[13].u0 + 1;
    arg0->v[0].v = menuDigitFont.rects[13].v0 + 1;
    arg0->v[1].u = menuDigitFont.rects[13].u1;
    arg0->v[1].v = menuDigitFont.rects[13].v1;
    return FldMenuNumberColorBuild(arg0 + 1, arg2, 0, arg3 + (s32)(sx * 20.0f), arg4, c2, sx, sy);
}
