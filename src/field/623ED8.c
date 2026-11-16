/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/menu.h"


extern void GsTexUpload(s32);

void FldMenuWinFrameDraw(FldMenuWin *win, s32 x, s32 y) {
    FldMenuQuad *p;
    u8 a;
    s32 i;

    if (win->alpha <= 0.0f) {
        return;
    }
    p = &win->quads[32];
    a = (s32)(win->alpha * 160.0f);
    if (win->flags & 0x20000) {
        p->type = 5;
        p->v[0].r = 0x3C;
        p->v[0].g = 0x96;
        p->v[0].b = 0xB4;
        p->v[0].a = a;
        p->v[2].r = 0x3C;
        p->v[2].g = 0x96;
        p->v[2].b = 0xB4;
        p->v[2].a = a;
        p->v[1].r = 0x50;
        p->v[1].g = 0xC8;
        p->v[1].b = 0x80;
        p->v[1].a = a;
        p->v[3].r = 0x50;
        p->v[3].g = 0xC8;
        p->v[3].b = 0x80;
        p->v[3].a = a;
        p->v[0].x = -0x60;
        p->v[0].y = -0x14;
        p->v[1].x = -0x60;
        p->v[1].y = 0x14;
        p->v[2].x = -0x30;
        p->v[2].y = -0x14;
        p->v[3].x = -0x30;
        p->v[3].y = 0x14;
        p->z = 0;
        p++;
        p->type = 5;
        p->v[0].r = 0x3C;
        p->v[0].g = 0x96;
        p->v[0].b = 0xB4;
        p->v[0].a = a;
        p->v[2].r = 0x3C;
        p->v[2].g = 0x96;
        p->v[2].b = 0xB4;
        p->v[2].a = a;
        p->v[1].r = 0x50;
        p->v[1].g = 0xC8;
        p->v[1].b = 0x80;
        p->v[1].a = a;
        p->v[3].r = 0x50;
        p->v[3].g = 0xC8;
        p->v[3].b = 0x80;
        p->v[3].a = a;
        p->v[0].x = -0x30;
        p->v[0].y = -0x14;
        p->v[1].x = -0x30;
        p->v[1].y = 0x14;
        p->v[2].y = -0x14;
        p->v[3].x = -0x10;
        p->v[2].x = -0x28;
        p->v[3].y = 0x4;
        p->z = 0;
        p[1].type = 0;
        GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1) - 2);
        GphBatchDrawQuadsColor(&win->quads[32], (a << 24) | 0xCCEBA4);
        GphBatchSetOffset(x - (win->w2 >> 1) - 1, y - (win->h2 >> 1) - 1);
        GphBatchDrawQuadsColor(&win->quads[32], (a << 24) | 0xCCEBA4);
        GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1) + 2);
        GphBatchDrawQuadsColor(&win->quads[32], a << 24);
        GphBatchSetOffset(x - (win->w2 >> 1) + 1, y - (win->h2 >> 1) + 1);
        GphBatchDrawQuadsColor(&win->quads[32], a << 24);
        GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1));
        GphBatchDrawQuads(&win->quads[32]);
        p = &win->quads[32];
    }
    if (!(win->flags & 0x400000)) {
        if (win->flags & 0x10000) {
            p = FldMenuWinBandBuild(p, 0, 2, win->w2, 0xE, a);
        } else {
            p = FldMenuWinBodyBuild(p, win, x, y, a);
        }
        p->type = 0;
        if (!(win->flags & 0x400000)) {
            GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1) - 2);
            GphBatchDrawQuadsColor(&win->quads[32], (a << 24) | 0xCCEBA4);
            GphBatchSetOffset(x - (win->w2 >> 1) - 1, y - (win->h2 >> 1) - 1);
            GphBatchDrawQuadsColor(&win->quads[32], (a << 24) | 0xCCEBA4);
            GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1) + 2);
            GphBatchDrawQuadsColor(&win->quads[32], a << 24);
            GphBatchSetOffset(x - (win->w2 >> 1) + 1, y - (win->h2 >> 1) + 1);
            GphBatchDrawQuadsColor(&win->quads[32], a << 24);
            GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1));
            GphBatchDrawQuads(&win->quads[32]);
        }
    }
    /* flags & 0x301000 read as one u64 with unk28: the original loads it with ld */
    if (*(u64 *)&win->unk28 & ((u64)0xC040 << 38)) {
        if (!(win->flags & 0x400000)) {
            p = &win->quads[32];
            if (win->flags & 0x1000) {
                p = FldMenuWinBandBuild(p, 0, 2, 0xA8, 0xC, a);
            }
            if (win->flags & 0x100000) {
                p = FldMenuWinBandBuild(p, win->w2 - 0xA8, 2, 0xA8, 0xC, a);
            }
            if (win->flags & 0x200000) {
                p = FldMenuWinBandBuild(p, 0, 0x10, 0xA8, 0xC, a);
            }
            p->type = 0;
            GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1) - 2);
            GphBatchDrawQuadsColor(&win->quads[32], (a << 24) | 0xCCEBA4);
            GphBatchSetOffset(x - (win->w2 >> 1) - 1, y - (win->h2 >> 1) - 1);
            GphBatchDrawQuadsColor(&win->quads[32], (a << 24) | 0xCCEBA4);
            GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1) + 2);
            GphBatchDrawQuadsColor(&win->quads[32], a << 24);
            GphBatchSetOffset(x - (win->w2 >> 1) + 1, y - (win->h2 >> 1) + 1);
            GphBatchDrawQuadsColor(&win->quads[32], a << 24);
            GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1));
            GphBatchDrawQuads(&win->quads[32]);
        }
    }
    i = 15; /* kept in a variable: the original indexes with it after the if/else */
    GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1));
    GphBatchDrawQuads(&win->quads[0]);
    if (win->layerTex[15] != 0 && win->layerQuad[15] != NULL) {
        if (win->styleFlags & 1) {
            GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1) - 6);
        } else {
            GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1));
        }
        GsTexUpload(win->layerTex[i]);
        GsTexPacketSend(win->layerTex[i]);
        GphBatchDrawQuads(win->layerQuad[i]);
    }
    for (i = 1; i < 15; i++) {
        if (win->layerTex[i] != 0 && win->layerQuad[i] != NULL) {
            if (win->styleFlags & 1) {
                GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1) - 6);
            } else {
                GphBatchSetOffset(x - (win->w2 >> 1), y - (win->h2 >> 1));
            }
            GsTexUpload(win->layerTex[i]);
            GsTexPacketSend(win->layerTex[i]);
            GphBatchDrawSprites(win->layerQuad[i]);
        }
    }
}
