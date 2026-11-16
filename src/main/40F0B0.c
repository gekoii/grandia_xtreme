#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n01.h"

void VuMtxSetRotZ(f32 (*)[4], s32);

void SprDrawRect(SprRect *r, s16 x, s16 y) {
    GphBatchQuad *e;
    SprState *c;
    SprState *c2;
    f32 m[4][4];
    f32 skew;
    s32 x1;
    f32 X0;
    f32 Y0;
    f32 X1;
    f32 Y1;

    c = &sprState;
    e = &sprQuadList[sprQuadCount];
    if ((c->angle & 0xFFFF) == 0) {
        e->v[0].x = e->v[1].x = c->ox + (f32)(c->offX + x) * sprState.sx;
        x1 = c->offX + r->w + x;
        e->v[0].y = e->v[2].y = c->oy + (f32)(c->offY + y) * c->sy;
        e->v[2].x = e->v[3].x = c->ox + (f32)x1 * sprState.sx;
        e->v[1].y = e->v[3].y = c->oy + (f32)(c->offY + r->h + y) * c->sy;
        skew = c->skewX * sprState.sx;
        e->v[0].x += skew;
        e->v[1].x -= skew;
        e->v[2].x += skew;
        e->v[3].x -= skew;
        skew = c->skewY * c->sy;
        e->v[0].y += skew;
        e->v[1].y += skew;
        e->v[2].y -= skew;
        e->v[3].y -= skew;
        if (e->v[0].x < 0 && e->v[2].x < 0) {
            return;
        }
        if (e->v[0].x > 640 && e->v[2].x > 640) {
            return;
        }
        if (e->v[0].y < 0 && e->v[1].y < 0) {
            return;
        }
        if (e->v[0].y > 448 && e->v[1].y > 448) {
            return;
        }
    } else {
        VuMtxSetRotZ(m, c->angle);
        X0 = (f32)(c->offX + x) * sprState.sx;
        X1 = (f32)(c->offX + r->w + x) * sprState.sx;
        Y0 = (f32)(c->offY + y) * c->sy;
        Y1 = (f32)(c->offY + r->h + y) * c->sy;
        e->v[0].x = m[0][0] * X0 + m[1][0] * Y0 + c->ox;
        e->v[0].y = m[0][1] * X0 + m[1][1] * Y0 + c->oy;
        e->v[1].x = m[0][0] * X0 + m[1][0] * Y1 + c->ox;
        e->v[1].y = m[0][1] * X0 + m[1][1] * Y1 + c->oy;
        e->v[2].x = m[0][0] * X1 + m[1][0] * Y0 + c->ox;
        e->v[2].y = m[0][1] * X1 + m[1][1] * Y0 + c->oy;
        e->v[3].x = m[0][0] * X1 + m[1][0] * Y1 + c->ox;
        e->v[3].y = m[0][1] * X1 + m[1][1] * Y1 + c->oy;
        if (e->v[0].x < 0 && e->v[1].x < 0 && e->v[2].x < 0 && e->v[3].x < 0) {
            return;
        }
        if (e->v[0].x > 640 && e->v[1].x > 640 && e->v[2].x > 640 && e->v[3].x > 640) {
            return;
        }
        if (e->v[0].y < 0 && e->v[1].y < 0 && e->v[2].y < 0 && e->v[3].y < 0) {
            return;
        }
        if (e->v[0].y > 448 && e->v[1].y > 448 && e->v[2].y > 448 && e->v[3].y > 448) {
            return;
        }
    }
    e->v[0].u = e->v[1].u = (sprState.flags & 1) ? r->u1 : r->u0;
    e->v[2].u = e->v[3].u = (sprState.flags & 1) ? r->u0 : r->u1;
    e->v[0].v = e->v[2].v = (sprState.flags & 2) ? r->v1 : r->v0;
    e->v[1].v = e->v[3].v = (sprState.flags & 2) ? r->v0 : r->v1;
    c2 = &sprState;
    e->type = c2->blend & 3;
    if (c2->flags & 4) {
        e->v[0].r = c2->col[0].r;
        e->v[0].g = c2->col[0].g;
        e->v[0].b = c2->col[0].b;
        e->v[0].a = c2->col[0].a;
        e->v[1].r = c2->col[1].r;
        e->v[1].g = c2->col[1].g;
        e->v[1].b = c2->col[1].b;
        e->v[1].a = c2->col[1].a;
        e->v[2].r = c2->col[2].r;
        e->v[2].g = c2->col[2].g;
        e->v[2].b = c2->col[2].b;
        e->v[2].a = c2->col[2].a;
        e->v[3].r = c2->col[3].r;
        e->v[3].g = c2->col[3].g;
        e->v[3].b = c2->col[3].b;
        e->v[3].a = c2->col[3].a;
    } else {
        e->v[0].r = c2->col[0].r;
        e->v[0].g = c2->col[0].g;
        e->v[0].b = c2->col[0].b;
        e->v[0].a = c2->col[0].a;
        e->v[1].r = c2->col[0].r;
        e->v[1].g = c2->col[0].g;
        e->v[1].b = c2->col[0].b;
        e->v[1].a = c2->col[0].a;
        e->v[2].r = c2->col[0].r;
        e->v[2].g = c2->col[0].g;
        e->v[2].b = c2->col[0].b;
        e->v[2].a = c2->col[0].a;
        e->v[3].r = c2->col[0].r;
        e->v[3].g = c2->col[0].g;
        e->v[3].b = c2->col[0].b;
        e->v[3].a = c2->col[0].a;
    }
    sprQuadCount++;
    if (sprQuadCount >= 0x20) {
        sprQuadList[sprQuadCount].type = 0;
        GphBatchDrawQuads(sprQuadList);
        sprQuadCount = 0;
    }
}
