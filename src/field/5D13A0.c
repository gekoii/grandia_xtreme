#include "common.h"
#include "protos/field.h"
#include "field/n15.h"
#include "field/n23.h"

void VuBezier3(VuVec4 *, VuVec4 *, f32);
void GsTexUpload(void *);

void TxtMsgCursorDraw(s32 arg0) {
    VuVec4 k[4];
    VuVec4 o1;
    VuVec4 o2;
    TxtMsgCursor *e;
    GphBatchQuad *s;
    s32 w;
    s32 h;
    s32 half;

    if (txtMsgCursorWork[arg0].used == 0 || TxtMsgCursorIsVisible() == 0) {
        return;
    }
    e = &txtMsgCursorWork[arg0];
    s = &e->spr;
    k[0].x = -16.0f;
    k[1].x = 0.0f;
    k[2].x = 0.0f;
    k[3].x = -16.0f;
    VuBezier3(&o1, k, e->u);
    k[0].x = 32.0f;
    k[1].x = 32.0f;
    k[2].x = 0.0f;
    k[3].x = 32.0f;
    VuBezier3(&o2, k, txtMsgCursorWork[arg0].v);
    s->type = 1;
    s->v[0].r = 0x80;
    s->v[0].g = 0x80;
    s->v[0].b = 0x80;
    s->v[0].a = 0x80;
    w = o2.x;
    h = o1.x;
    half = w / 2;
    s->v[0].x = txtMsgCursorWork[arg0].x + 16 - half;
    s->z = 0;
    e->unk50 = 0;
    s->v[0].y = txtMsgCursorWork[arg0].y + h;
    s->v[1].x = txtMsgCursorWork[arg0].x + 16 - half + w;
    s->v[1].y = txtMsgCursorWork[arg0].y + h + 32;
    s->v[0].u = 0xE0;
    s->v[0].v = 0x88;
    s->v[1].u = 0x100;
    s->v[1].v = 0xA8;
    GsTexUpload(&fldMenuTexIcons);
    GsTexPacketSend(&fldMenuTexIcons);
    GphBatchDrawSprites(&txtMsgCursorWork[arg0].spr);
}
