#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniSpriteDrawAnim(MiniPanel *h, MiniSprite *o) {
    GphBatchQuad q[4];
    f32 fx;
    f32 fy;
    f32 dx;
    f32 dy;
    u8 r;
    u8 g;
    u8 b;

    q[0].type = 1;
    fx = o->x + o->waveX + o->ofsX;
    fy = o->y + o->waveY + o->ofsY;
    fx = h->x + fx * h->sx;
    fy = h->y + fy * h->sy;
    GphAnimBuildSprite(&o->anim, q, fx, fy, o->angle);
    dx = (q[0].v[3].x - q[0].v[0].x) * (o->sx - 1.0f) * 0.5f;
    r = o->r;
    q[0].v[0].r = r;
    g = o->g;
    b = o->b;
    q[0].v[0].g = g;
    q[0].v[0].b = b;
    q[0].v[1].r = r;
    q[0].v[1].g = g;
    q[0].v[1].b = b;
    dy = (q[0].v[3].y - q[0].v[0].y) * (o->sy - 1.0f) * 0.5f;
    q[0].v[0].x = q[0].v[0].x - dx;
    q[0].v[0].y = q[0].v[0].y - dy;
    q[0].v[1].x = q[0].v[1].x - dx;
    q[0].v[1].y = q[0].v[1].y + dy;
    q[0].v[2].x = q[0].v[2].x + dx;
    q[0].v[2].y = q[0].v[2].y - dy;
    q[0].v[3].x = q[0].v[3].x + dx;
    q[0].v[3].y = q[0].v[3].y + dy;
    q[0].v[0].a = o->a * (o->alphaScale * (1.0f / 128.0f)) * (h->alpha * (1.0f / 128.0f));
    q[0].v[1].a = o->a * (o->alphaScale * (1.0f / 128.0f)) * (h->alpha * (1.0f / 128.0f));
    q[0].v[2].r = r;
    q[0].v[2].g = g;
    q[0].v[2].b = b;
    q[0].v[3].r = r;
    q[0].v[3].g = g;
    q[0].v[3].b = b;
    q[0].v[2].a = o->a * (o->alphaScale * (1.0f / 128.0f)) * (h->alpha * (1.0f / 128.0f));
    q[0].v[3].a = o->a * (o->alphaScale * (1.0f / 128.0f)) * (h->alpha * (1.0f / 128.0f));
    if (o->topLeft == 0) {
        fx -= q[0].v[0].x;
        fy -= q[0].v[0].y;
        q[0].v[0].x = q[0].v[0].x + fx;
        q[0].v[0].y = q[0].v[0].y + fy;
        q[0].v[1].x = q[0].v[1].x + fx;
        q[0].v[1].y = q[0].v[1].y + fy;
        q[0].v[2].x = q[0].v[2].x + fx;
        q[0].v[2].y = q[0].v[2].y + fy;
        q[0].v[3].x = q[0].v[3].x + fx;
        q[0].v[3].y = q[0].v[3].y + fy;
    }
    q[1].type = 0;
    GphBatchDrawQuads(q);
}
