#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n02.h"

f32 VuSinAngle(s32);
extern SprRect menuLrArrowSprites[];

void MenuLrArrowsDraw(void) {
    f32 a;
    f32 b;
    f32 c;
    SprState *p;
    SprState *r;
    MenuLrArrows *q;

    q = &menuLrArrows;
    a = VuSinAngle(menuWidgetFrame << 10);
    c = q->flashTimer * 0.03125f;
    b = a * 10.0f;
    c = VuCosAngle(c * 65536.0f * c);
    SprBegin((f32)(q->x - q->halfWidth) - b, q->y);
    p = &sprState;
    p->sx = 1.5f;
    if (q->flashSide == 1) {
        p->sy = c * 1.5f;
    } else {
        p->sy = 1.5f;
    }
    SprDrawRect(&menuLrArrowSprites[0], -menuLrArrowSprites[0].w / 2, -menuLrArrowSprites[0].h / 2);
    r = &sprState;
    r->ox = (f32)(q->x + q->halfWidth) + b;
    if (q->flashSide == 2) {
        r->sy = c * 1.5f;
    } else {
        r->sy = 1.5f;
    }
    SprDrawRect(&menuLrArrowSprites[1], -menuLrArrowSprites[1].w / 2, -menuLrArrowSprites[1].h / 2);
    SprFlush();
}
