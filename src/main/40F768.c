#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n01.h"


void SprDrawRectFast(SprRect *r, s16 x, s16 y) {
    GphBatchQuad *e;
    s32 x1;

    e = &sprRectList[sprRectCount];
    e->v[0].x = sprState.ox + (f32)(sprState.offX + x) * sprState.sx;
    x1 = sprState.offX + r->w + x;
    e->v[0].y = sprState.oy + (f32)(sprState.offY + y) * sprState.sy;
    e->v[1].x = sprState.ox + (f32)x1 * sprState.sx;
    e->v[1].y = sprState.oy + (f32)(sprState.offY + r->h + y) * sprState.sy;
    if (e->v[0].x < 0 && e->v[1].x < 0) {
        return;
    }
    if (e->v[0].x > 640 && e->v[1].x > 640) {
        return;
    }
    if (e->v[0].y < 0 && e->v[1].y < 0) {
        return;
    }
    if (e->v[0].y > 448 && e->v[1].y > 448) {
        return;
    }
    e->v[0].u = (sprState.flags & 1) ? r->u1 : r->u0;
    e->v[1].u = (sprState.flags & 1) ? r->u0 : r->u1;
    e->v[0].v = (sprState.flags & 2) ? r->v1 : r->v0;
    e->v[1].v = (sprState.flags & 2) ? r->v0 : r->v1;
    e->type = 1;
    e->v[0].r = sprState.col[0].r;
    e->v[0].g = sprState.col[0].g;
    e->v[0].b = sprState.col[0].b;
    e->v[0].a = sprState.col[0].a;
    sprRectCount++;
    if (sprRectCount >= 0x20) {
        sprRectList[sprRectCount].type = 0;
        GphBatchDrawSprites(sprRectList);
        sprRectCount = 0;
    }
}
