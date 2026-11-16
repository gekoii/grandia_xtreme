#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void GsTexUpload(void *);

void FldOverlayImgDraw(void) {
    FldOverlayImg *t = fldOverlayImg;
    GphBatchQuad *p;

    if (t != NULL && t->u.s.count != 0 && (t->u.all & 0xFFFFFFFF00000000ULL)) {
        p = &t->spr;
        t->spr.type = 1;
        p->v[0].r = 0x80;
        p->v[0].g = 0x80;
        p->v[0].b = 0x80;
        p->v[0].a = fldOverlayImg->u.s.alpha;
        p->z = 0;
        t->sprEnd = 0;
        p->v[0].x = 0;
        p->v[0].y = 0;
        p->v[1].x = 0x100;
        p->v[1].y = 0x100;
        p->v[0].u = 0;
        p->v[0].v = 0;
        p->v[1].u = 0x100;
        p->v[1].v = 0x100;
        GsTexUpload(FldOverlayImgGetTex(0));
        GsTexPacketSend(FldOverlayImgGetTex(0));
        GphBatchSetOffset(0x170, 0xB0);
        GphBatchDrawSprites(&fldOverlayImg->spr);
        GphBatchSetOffset(0, 0);
    }
}
