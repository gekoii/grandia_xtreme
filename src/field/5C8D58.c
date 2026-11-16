#include "common.h"
#include "grxsys.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n14.h"

#define G (&txtGlyphLayout[id] + 3)

void TxtGlyphLayoutAddChar(u8 id, u16 ch, s32 x, s32 y) {
    struct {
        s8 c0;
        s8 c1;
        s32 code;
    } t;
    GphBatchQuad *it;
    s32 t2;
    s32 t3;
    s16 w;
    s16 col;
    u8 c3;
    u8 c2;
    u8 c0;
    u8 c1;

    t.code = (u16)TxtMsgCharToGlyph(ch);
    if (x == 0) {
        G->x = 0;
        if (y == 0) {
            G->y = 0;
        }
    }
    TxtGlyphGetMetrics(t.code, (u8 *)&t.c0, (u8 *)&t.c1, sysGrxsysPac->glyphTable);
    it = &txtGlyphLayout[id + 3].sprites[G->count * 2];
    w = (f32)(t.c0 - t.c1) * 24.0f * 0.0625f;
    if (x >= 0x289) {
        col = x / 24 - 0x1C;
        if (x == 0x2A0) {
            G->y++;
        }
    } else {
        col = x / 24;
    }
    /* s32 code: the caller does not mask it to u16 (2 words) */
    ((void (*)(u8, s32, u8, u8))TxtGlyphQueuePush)(id + 3, t.code, col, G->y + y / 24);
    c3 = 0x20;
    if (evtMsgCtl->flags & 0x20) {
        c3 = 0x80;
    }
    it->v[0].a = c3;
    it->v[0].r = 0;
    it->v[0].g = 0;
    c0 = 0x80;
    it->type = 1;
    it->v[0].b = 0;
    it->v[0].x = G->x + 18;
    it->v[0].y = y + 10;
    t2 = w + 18; it->v[1].x = G->x + t2;
    it->v[1].y = y + 34;
    it->z = 0;
    c2 = 0x80;
    it->v[0].u = t.c1 + col * 18;
    it->v[0].v = (G->y + y / 24) * 18;
    it->v[1].u = t.c1 + col * 18 + (t.c0 - t.c1);
    c1 = 0x80;
    it->v[1].v = (G->y + y / 24) * 18 + 16;
    if ((evtMsgCtl->flags & 0x20) && evtMsgCtl->choice * 24 == y) {
        it->v[0].r = 0x40;
        it->v[0].g = 0x10;
        it->v[0].b = 0x20;
        c0 = 0x80;
        c1 = 0x80;
        c2 = 0;
        it->v[0].a = 0x60;
    }
    it++;
    it->type = 1;
    it->v[0].r = c0;
    it->v[0].g = c1;
    it->v[0].b = c2;
    it->v[0].a = c3;
    it->v[0].x = G->x + 16;
    it->v[0].y = y + 8;
    t3 = w + 16; it->v[1].x = G->x + t3;
    it->z = 0;
    it->v[1].y = y + 32;
    it->v[0].u = t.c1 + col * 18;
    it->v[0].v = (G->y + y / 24) * 18;
    it->v[1].u = t.c1 + col * 18 + (t.c0 - t.c1);
    it->v[1].v = (G->y + y / 24) * 18 + 16;
    G->count++;
    G->x += w + 2;
    it[1].type = 0;
}
