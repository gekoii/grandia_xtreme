/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "main/n07.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec;

void VuVecLerp(Vec *, Vec *, Vec *, f32);
void GsQuadRotate(GphBatchQuad *, s32, s32, s32, s32, f32);

void GphAnimBuildSprite(GphAnimState *anim, GphBatchQuad *spr, s32 x, s32 y, f32 ang) {
    Vec from;
    Vec to;
    Vec out;
    GphAnimRect *e;
    GphAnimKey *fr;
    GphAnimKey *nf;
    s32 w;
    s32 h;
    s32 a;
    s32 r;
    s32 g;
    s32 b;

    e = GphAnimGetCellRect(anim->cell);
    if (e == NULL) {
        return;
    }
    fr = GphAnimGetKey(anim->key);
    w = e->w;
    h = e->h;
    spr->v[0].u = e->x;
    spr->v[0].v = e->y;
    spr->v[1].u = e->x;
    spr->v[1].v = e->y + h;
    spr->v[2].u = e->x + w;
    spr->v[2].v = e->y;
    spr->v[3].u = e->x + w;
    spr->v[3].v = e->y + h;
    if (fr == NULL) {
        GsQuadRotate(spr, x, y, w, h, ang);
        return;
    }
    if (!(gphCurAnimSet->flags & 4)) {
        x += fr->x;
        y += fr->x;
        w = e->w * fr->scaleX;
        h = e->h * fr->scaleY;
        ang += fr->rot;
        spr->v[0].r = fr->color >> 24;
        spr->v[0].g = fr->color >> 16;
        spr->v[0].b = fr->color >> 8;
        spr->v[0].a = fr->color;
        spr->v[1].r = fr->color >> 24;
        spr->v[1].g = fr->color >> 16;
        spr->v[1].b = fr->color >> 8;
        spr->v[1].a = fr->color;
        spr->v[2].r = fr->color >> 24;
        spr->v[2].g = fr->color >> 16;
        spr->v[2].b = fr->color >> 8;
        spr->v[2].a = fr->color;
        spr->v[3].r = fr->color >> 24;
        spr->v[3].g = fr->color >> 16;
        spr->v[3].b = fr->color >> 8;
        spr->v[3].a = fr->color;
    } else {
        nf = GphAnimGetNextKey(anim->key);
        from.x = fr->x;
        from.y = fr->y;
        from.z = fr->rot;
        to.x = nf->x;
        to.y = nf->y;
        to.z = nf->rot;
        VuVecLerp(&out, &from, &to, anim->time / fr->wait);
        x += (s32)out.x;
        y += (s32)out.y;
        ang += out.z;
        from.x = fr->color & 0xFF;
        from.y = e->w * fr->scaleX;
        from.z = e->h * fr->scaleY;
        to.x = nf->color & 0xFF;
        to.y = e->w * nf->scaleX;
        to.z = e->h * nf->scaleY;
        VuVecLerp(&out, &from, &to, anim->time / fr->wait);
        a = out.x;
        w = out.y;
        h = out.z;
        from.x = (fr->color >> 24) & 0xFF;
        from.y = (fr->color >> 16) & 0xFF;
        from.z = (fr->color >> 8) & 0xFF;
        to.x = (nf->color >> 24) & 0xFF;
        to.y = (nf->color >> 16) & 0xFF;
        to.z = (nf->color >> 8) & 0xFF;
        VuVecLerp(&out, &from, &to, anim->time / fr->wait);
        r = out.x;
        g = out.y;
        b = out.z;
        spr->v[0].a = a;
        spr->v[0].r = r;
        spr->v[0].g = g;
        spr->v[0].b = b;
        spr->v[1].r = r;
        spr->v[1].g = g;
        spr->v[1].b = b;
        spr->v[1].a = a;
        spr->v[2].r = r;
        spr->v[3].r = r;
        spr->v[2].g = g;
        spr->v[3].g = g;
        spr->v[2].b = b;
        spr->v[3].b = b;
        spr->v[2].a = a;
        spr->v[3].a = a;
    }
    GsQuadRotate(spr, x, y, w, h, ang);
}
