/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n02.h"

extern SprRect menuFrameParts[];

void MenuFrameDraw(MenuFrame *a) {
    if (a->alpha <= 0.0f) {
        return;
    }
    GphTexBind(0xED);
    SprBegin(a->x, a->y);
    sprState.sx = a->sx;
    sprState.angle = a->angle;
    sprState.sy = a->sy;
    SprDrawColorQuad(&a->bg, a->alpha);
    sprState.col[0].a = a->alpha * 128.0f;
    sprState.offX = -a->ox;
    sprState.offY = -a->oy;
    menuFrameParts[5].h = a->h - 12;
    SprDrawRect(&menuFrameParts[5], -6, 14);
    SprDrawRect(&menuFrameParts[5], a->w, 14);
    SprDrawRect(&menuFrameParts[0], -10, -10);
    menuFrameParts[1].w = a->w / 2 - 8;
    if (menuFrameParts[1].w > 0) {
        SprDrawRect(&menuFrameParts[1], 0, -10);
    } else {
        menuFrameParts[1].w = 0;
    }
    SprDrawRect(&menuFrameParts[2], menuFrameParts[1].w, -10);
    menuFrameParts[3].w = (s16)(a->w - 8) - menuFrameParts[1].w;
    SprDrawRect(&menuFrameParts[3], menuFrameParts[1].w + 8, -10);
    SprDrawRect(&menuFrameParts[4], a->w, -10);
    SprDrawRect(&menuFrameParts[6], -10, a->h);
    menuFrameParts[7].w = a->w * 0.75 - 8.0;
    if (menuFrameParts[7].w > 0) {
        SprDrawRect(&menuFrameParts[7], 0, a->h);
    } else {
        menuFrameParts[7].w = 0;
    }
    SprDrawRect(&menuFrameParts[8], menuFrameParts[7].w, a->h);
    menuFrameParts[9].w = (s16)(a->w - 8) - menuFrameParts[7].w;
    SprDrawRect(&menuFrameParts[9], menuFrameParts[7].w + 8, a->h);
    SprDrawRect(&menuFrameParts[10], a->w, a->h);
    SprFlush();
}
