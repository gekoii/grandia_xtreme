#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n01.h"

void VuMtxSetRotZ(f32 (*)[4], s32);

void SprDrawColorQuad(SprColorQuad *q, f32 alpha) {
    GphBatchQuad *p = &sprQuadList[sprQuadCount];
    f32 m[4][4];

    if (alpha <= 0.0f) {
        return;
    }
    if (!(sprState.angle & 0xFFFF)) {
        p->v[0].x = p->v[1].x = sprState.ox + (sprState.offX - q->originX) * sprState.sx;
        p->v[2].x = p->v[3].x = sprState.ox + (sprState.offX + q->w - q->originX) * sprState.sx;
        p->v[0].y = p->v[2].y = sprState.oy + (sprState.offY - q->originY) * sprState.sy;
        p->v[1].y = p->v[3].y = sprState.oy + (sprState.offY + q->h - q->originY) * sprState.sy;
        if (p->v[0].x < 0 && p->v[2].x < 0) {
            return;
        }
        if (p->v[0].x > 640 && p->v[2].x > 640) {
            return;
        }
        if (p->v[0].y < 0 && p->v[1].y < 0) {
            return;
        }
        if (p->v[0].y > 448 && p->v[1].y > 448) {
            return;
        }
    } else {
        f32 xl, xr, yt, yb;

        VuMtxSetRotZ(m, sprState.angle);
        xl = (sprState.offX - q->originX) * sprState.sx;
        xr = (sprState.offX + q->w - q->originX) * sprState.sx;
        yt = (sprState.offY - q->originY) * sprState.sy;
        yb = (sprState.offY + q->h - q->originY) * sprState.sy;
        p->v[0].x = m[0][0] * xl + m[1][0] * yt + sprState.ox;
        p->v[0].y = m[0][1] * xl + m[1][1] * yt + sprState.oy;
        p->v[1].x = m[0][0] * xl + m[1][0] * yb + sprState.ox;
        p->v[1].y = m[0][1] * xl + m[1][1] * yb + sprState.oy;
        p->v[2].x = m[0][0] * xr + m[1][0] * yt + sprState.ox;
        p->v[2].y = m[0][1] * xr + m[1][1] * yt + sprState.oy;
        p->v[3].x = m[0][0] * xr + m[1][0] * yb + sprState.ox;
        p->v[3].y = m[0][1] * xr + m[1][1] * yb + sprState.oy;
        if (p->v[0].x < 0 && p->v[1].x < 0 && p->v[2].x < 0 && p->v[3].x < 0) {
            return;
        }
        if (p->v[0].x > 640 && p->v[1].x > 640 && p->v[2].x > 640 && p->v[3].x > 640) {
            return;
        }
        if (p->v[0].y < 0 && p->v[1].y < 0 && p->v[2].y < 0 && p->v[3].y < 0) {
            return;
        }
        if (p->v[0].y > 448 && p->v[1].y > 448 && p->v[2].y > 448 && p->v[3].y > 448) {
            return;
        }
    }
    p->v[0].r = q->col[0].b[3];
    p->v[0].g = q->col[0].b[2];
    p->v[0].b = q->col[0].b[1];
    p->v[0].a = q->col[0].b[0] * alpha;
    p->v[1].r = q->col[1].b[3];
    p->v[1].g = q->col[1].b[2];
    p->v[1].b = q->col[1].b[1];
    p->v[1].a = q->col[1].b[0] * alpha;
    p->v[2].r = q->col[2].b[3];
    p->v[2].g = q->col[2].b[2];
    p->v[2].b = q->col[2].b[1];
    p->v[2].a = q->col[2].b[0] * alpha;
    p->v[3].r = q->col[3].b[3];
    p->v[3].g = q->col[3].b[2];
    p->v[3].b = q->col[3].b[1];
    p->v[3].a = q->col[3].b[0] * alpha;
    p->type = (sprState.blend & 3) | 4;
    sprQuadCount++;
    if (sprQuadCount >= 32) {
        sprQuadList[sprQuadCount].type = 0;
        GphBatchDrawQuads(sprQuadList);
        sprQuadCount = 0;
    }
}
