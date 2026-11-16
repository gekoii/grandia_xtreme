#include "common.h"
#include "protos/main.h"
#include "main/n07.h"

s32 VuFloatToInt(f32);

void GphFadeDraw(GphFade *f) {
    GphBatchQuad *p = f->prim;
    GphBatchQuad *q = p;

    if (f->mode == 0) {
        p->type = ((f->unk2 + 1) & 3) | 4;
        p->v[0].r = VuFloatToInt(f->color[0]);
        p->v[0].g = VuFloatToInt(f->color[1]);
        p->v[0].b = VuFloatToInt(f->color[2]);
        p->v[0].a = VuFloatToInt(f->color[3]);
        p->v[1].r = VuFloatToInt(f->color[0]);
        p->v[1].g = VuFloatToInt(f->color[1]);
        p->v[1].b = VuFloatToInt(f->color[2]);
        p->v[1].a = VuFloatToInt(f->color[3]);
        p->v[2].r = VuFloatToInt(f->color[0]);
        p->v[2].g = VuFloatToInt(f->color[1]);
        p->v[2].b = VuFloatToInt(f->color[2]);
        p->v[2].a = VuFloatToInt(f->color[3]);
        p->v[3].r = VuFloatToInt(f->color[0]);
        p->v[3].g = VuFloatToInt(f->color[1]);
        p->v[3].b = VuFloatToInt(f->color[2]);
        p->v[3].a = VuFloatToInt(f->color[3]);
        q = p + 1;
        p->v[0].x = 0;
        p->v[0].y = 0;
        p->v[1].x = 0;
        p->v[1].y = 448;
        p->v[2].x = 640;
        p->v[2].y = 0;
        p->v[3].x = 640;
        p->v[3].y = 448;
        p->z = 0;
    } else {
        f->active = 0;
    }
    q->type = 0;
    if (VuFloatToInt(f->color[3]) != 0) {
        GphBatchDrawQuads(p);
    }
}
