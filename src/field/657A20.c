#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

extern void GsTexUpload(void *);

void FldPictureDraw(void) {
    FldPictureWork *w;
    GphBatchQuad *q;
    FldPictureWork *w2;
    s32 i;

    w = fldPictureWork;
    if (w->shown == 0) {
        return;
    }
    if (w->dim != 0) {
        q = &w->prim;
        w->prim.type = 5;
        q->v[0].r = 0;
        q->v[0].g = 0;
        q->v[0].b = 0;
        q->v[0].a = fldPictureWork->dim;
        q->v[1].r = 0;
        q->v[1].g = 0;
        q->v[1].b = 0;
        q->v[1].a = fldPictureWork->dim;
        q->v[2].r = 0;
        q->v[2].g = 0;
        q->v[2].b = 0;
        q->v[2].a = fldPictureWork->dim;
        q->v[3].r = 0;
        q->v[3].g = 0;
        q->v[3].b = 0;
        q->v[3].a = fldPictureWork->dim;
        q->v[0].x = 0;
        q->v[0].y = 0;
        q->v[1].x = 0;
        q->v[1].y = 448;
        q->v[2].x = 640;
        q->v[2].y = 0;
        q->v[3].x = 640;
        q->v[3].y = 448;
        q->z = 0;
        w->primEnd = 0;
        GphBatchSetOffset(0, 0);
        GphBatchDrawQuads(&fldPictureWork->prim);
    }
    for (i = 0; i < 2; i++) {
        if (fldPictureWork->slots[i] == 0) {
            continue;
        }
        GsTexUpload(FldOverlayImgGetTex(fldPictureWork->slots[i] - 1));
        GsTexPacketSend(FldOverlayImgGetTex(fldPictureWork->slots[i] - 1));
        w2 = fldPictureWork;
        q = &w2->prim;
        q->type = 1;
        q->v[0].r = 0x80;
        q->v[0].g = 0x80;
        q->v[0].b = 0x80;
        q->v[0].a = fldPictureWork->out[i][2];
        q->z = 0;
        w2->primEnd = 0;
        q->v[0].x = 0;
        q->v[0].y = 0;
        q->v[1].x = 0x200;
        q->v[1].y = 0x180;
        q->v[0].u = 0;
        q->v[0].v = 0;
        q->v[1].u = 0x200;
        q->v[1].v = 0x180;
        GphBatchSetOffset(fldPictureWork->out[i][0], fldPictureWork->out[i][1]);
        GphBatchDrawSprites(&fldPictureWork->prim);
    }
}
