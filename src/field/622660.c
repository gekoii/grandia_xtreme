#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/menu.h"
#include "field/n23.h"

typedef int s128 __attribute__((mode(TI)));

typedef union {
    s128 q;
    f32 f[4];
} Q;


void VuBezier3(Q *dst, Q *m, f32 t);
void GsTexUpload(void *);

void FldMenuWinDraw(void) {
    FldMenuQuad prims[3];
    Q m[4];
    Q q;
    s32 i;
    s32 n;
    FldMenuWin *e;
    FldMenuQuad *p;
    s16 sx;
    s16 sy;

    n = 0;
    for (i = 0; i < 18; i++) {
        e = &fldMenuWins[i];
        if (e->active != 0) {
            sx = (s32)(e->pos[0] + 0.5f);
            sy = (s32)(e->pos[1] + 0.5f);
            FldMenuWinFrameDraw(e, sx, sy);
            if (e->arrowL != 0 || e->arrowR != 0) {
                p = prims;
                m[1].f[0] = 0.0f;
                m[0].f[0] = -16.0f;
                m[3].f[0] = -16.0f;
                m[2].f[0] = 0.0f;
                VuBezier3(&q, m, e->unk38);
                GphBatchSetOffset(sx - (e->w2 >> 1), sy - (e->h2 >> 1));
                if (e->arrowL != 0) {
                    s32 v = (s32)q.f[0];
                    p->type = 1;
                    p->v[0].r = 0xAD; p->v[0].g = 0x95; p->v[0].b = 0x60; p->v[0].a = (s32)(e->alpha * 128.0f);
                    p->v[1].r = 0xAD; p->v[1].g = 0x95; p->v[1].b = 0x60; p->v[1].a = (s32)(e->alpha * 128.0f);
                    p->v[2].r = 0xAD; p->v[2].g = 0x95; p->v[2].b = 0x60; p->v[2].a = (s32)(e->alpha * 128.0f);
                    p->v[3].r = 0xAD; p->v[3].g = 0x95; p->v[3].b = 0x60; p->v[3].a = (s32)(e->alpha * 128.0f);
                    p->v[0].x = e->w2 - 0x10;
                    p->v[0].y = -0x20 - v;
                    p->v[1].x = e->w2 - 0x10;
                    p->v[1].y = -v;
                    p->v[2].x = e->w2 + 0x10;
                    p->v[2].y = -0x20 - v;
                    p->v[3].x = e->w2 + 0x10;
                    p->v[3].y = -v;
                    p->z = 0;
                    p->v[0].u = 0xE0;
                    p->v[0].v = 0x88;
                    p->v[1].u = 0xE0;
                    p->v[1].v = 0xA8;
                    p->v[2].u = 0x100;
                    p->v[2].v = 0x88;
                    p->v[3].u = 0x100;
                    p->v[3].v = 0xA8;
                    if (p->v[0].y != p->v[1].y) {
                        p->v[0].y ^= p->v[1].y;
                        p->v[1].y ^= p->v[0].y;
                        p->v[0].y ^= p->v[1].y;
                    }
                    if (p->v[2].y != p->v[3].y) {
                        p->v[2].y ^= p->v[3].y;
                        p->v[3].y ^= p->v[2].y;
                        p->v[2].y ^= p->v[3].y;
                    }
                    p++;
                }
                if (e->arrowR != 0) {
                    s32 v;

                    p->type = 1;
                    p->v[0].r = 0xAD; p->v[0].g = 0x95; p->v[0].b = 0x60; p->v[0].a = (s32)(e->alpha * 128.0f);
                    p->v[1].r = 0xAD; p->v[1].g = 0x95; p->v[1].b = 0x60; p->v[1].a = (s32)(e->alpha * 128.0f);
                    p->v[2].r = 0xAD; p->v[2].g = 0x95; p->v[2].b = 0x60; p->v[2].a = (s32)(e->alpha * 128.0f);
                    p->v[3].r = 0xAD; p->v[3].g = 0x95; p->v[3].b = 0x60; p->v[3].a = (s32)(e->alpha * 128.0f);
                    v = (s32)q.f[0];
                    p->v[0].x = e->w2 - 0x10;
                    p->v[0].y = e->h2 + v;
                    p->v[1].x = e->w2 - 0x10;
                    p->v[1].y = e->h2 + v + 0x20;
                    p->v[2].x = e->w2 + 0x10;
                    p->v[2].y = e->h2 + v;
                    p->v[3].x = e->w2 + 0x10;
                    p->v[3].y = e->h2 + v + 0x20;
                    p->z = 0;
                    p->v[0].u = 0xE0;
                    p->v[0].v = 0x88;
                    p->v[1].u = 0xE0;
                    p->v[1].v = 0xA8;
                    p->v[2].u = 0x100;
                    p->v[2].v = 0x88;
                    p->v[3].u = 0x100;
                    p->v[3].v = 0xA8;
                    p++;
                }
                p->type = 0;
                GsTexUpload(&fldMenuTexIcons);
                GsTexPacketSend(&fldMenuTexIcons);
                GphBatchDrawQuads(prims);
            }
            if (e->glyphCount != 0) {
                GphBatchSetOffset(sx - (e->w2 >> 1), sy - (e->h2 >> 1));
                TxtWinTextureBind(e->txtWinId);
                GphBatchDrawSprites(&e->quads[82]) /* glyph sprites at +0x8D10 */;
            }
            if ((void *)e->layerTex[0] != NULL && e->layerQuad[0] != NULL) {
                if (e->styleFlags & 1) {
                    GphBatchSetOffset(sx - (e->w2 >> 1), sy - (e->h2 >> 1) - 6);
                } else {
                    GphBatchSetOffset(sx - (e->w2 >> 1), sy - (e->h2 >> 1));
                }
                GsTexUpload((void *)e->layerTex[0]);
                GsTexPacketSend((void *)e->layerTex[0]);
                GphBatchDrawSprites(e->layerQuad[0]);
            }
            GphBatchSetOffset(0, 0);
            n++;
        }
    }
    if (n != 0) {
        MemHeapSelect(sysState.battleHeap);
        MenuHelpDraw();
        MenuWidgetsDraw();
        if (fldMenuCursor->lrLock) {
            GphTexBind(0xEC);
            MenuLrArrowsDraw();
        }
        MemHeapSelect(sysState.dataHeap);
    }
}
