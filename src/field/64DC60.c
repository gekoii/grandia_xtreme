#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/n24.h"
#include "field/n25.h"

/* sprState's colour written through 8-bit bitfields: plain col[0].a stores schedule differently (6 words off) */
typedef struct {
    u8 pad0[0x10];
    u32 r : 8;
    u32 g : 8;
    u32 b : 8;
    u32 a : 8;
} SprStateBits;

/* A one-byte non-char type: its loads get their own alias set, so they do
   not conflict with the SprState colour store below (needed for sched2 to
   order the alpha loads like the original). */
typedef enum __attribute__((packed)) { ALPHA_MAX = 255 } Alpha;


void MiniPanelsDraw(u8 arg0, MiniPanel *groups, u8 n) {
    u8 layer;
    u32 i;
    u32 j;
    s32 any;
    s32 flagA;
    s32 flagB;
    s32 last;
    s32 last4;
    MiniPanel *g;
    MiniSprite *o;
    SprRect *tbl;
    u8 c;
    u8 a;
    f32 hw;
    f32 hh;
    f32 x;
    f32 y;

    layer = 0;
    do {
        any = 0;
        for (i = 0; i < n; i++) {
            g = &groups[i];
            if (g->active == 0) {
                continue;
            }
            if (g->maxLayer < layer) {
                continue;
            }
            flagA = 0;
            any = 1;
            flagB = 0;
            if (layer == 0) {
                for (j = 0; j < g->count; j++) {
                    o = &g->sprites[j];
                    if (o->active != 0) {
                        if (o->drawFlags & 2) {
                            flagA = 1;
                        }
                        if (o->drawFlags & 4) {
                            flagB = 1;
                        }
                        if ((o->drawFlags & 0x10) && (o->moveFlags & 1)) {
                            MiniSparkleBurst(g, o, 3, 0);
                        }
                    }
                }
            }
            if (miniUi->curTex != g->tex) {
                GphTexReplace(miniLayout->images[g->tex], arg0);
                GphTexBind(arg0);
                miniUi->curTex = g->tex;
            }
            SprBegin(g->x, g->y);
            SprSetScale(g->sx, g->sy);
            for (j = 0; j < g->count; j++) {
                o = &g->sprites[j];
                if (o->active == 0) {
                    continue;
                }
                if (o->layer != layer) {
                    continue;
                }
                if (!(o->drawFlags & 1)) {
                    continue;
                }
                if (o->animNo != 0) {
                    continue;
                }
                if (o->icon != 0) {
                    continue;
                }
                ((SprStateBits *)&sprState)->a = o->a * (o->alphaScale / 128.0f) * (*(Alpha *)&g->alpha / 128.0f);
                if (o->rectIsList == 1) {
                    SprPushTransform(o->angle, (f32)(o->x + o->waveX + o->ofsX), (f32)(o->y + o->waveY + o->ofsY), o->sx, o->sy);
                    SprDrawRectList(&miniLayout->lists[miniLayout->listIdx[o->rect]]);
                } else {
                    if (o->topLeft == 0) {
                        hw = miniLayout->rects[o->rect].w * 0.5f;
                        hh = miniLayout->rects[o->rect].h * 0.5f;
                        x = (f32)(o->x + o->waveX + o->ofsX) + hw;
                        y = (f32)(o->y + o->waveY + o->ofsY) + hh;
                    } else {
                        hw = miniLayout->rects[o->rect].w * o->sx * 0.5f;
                        hh = miniLayout->rects[o->rect].h * o->sy * 0.5f;
                        x = (f32)(o->x + o->waveX + o->ofsX) + hw;
                        y = (f32)(o->y + o->waveY + o->ofsY) + hh;
                    }
                    SprPushTransform(o->angle, x, y, o->sx, o->sy);
                    if (o->stretchRect != 0) {
                        SprDrawRect(&miniLayout->rects[o->stretchRect], -hw, -hh);
                    } else {
                        SprDrawRect(&miniLayout->rects[o->rect], -hw, -hh);
                    }
                }
                SprPopTransform();
            }
            SprFlush();
            if (g->showIcons == 1) {
                tbl = (SprRect *)menuItemIcons;
                last = 0;
                SprBegin(g->x, g->y);
                SprSetScale(g->sx, g->sy);
                for (j = 0; j < g->count; j++) {
                    o = &g->sprites[j];
                    if (o->active == 0) {
                        continue;
                    }
                    if (o->layer != layer) {
                        continue;
                    }
                    if (!(o->drawFlags & 1)) {
                        continue;
                    }
                    if (o->icon != 0) {
                        c = o->icon;
                        if (c != last) {
                            GphTexBind(c);
                            last = c;
                            miniUi->curTex = -1;
                        }
                        ((SprStateBits *)&sprState)->a = o->a * (o->alphaScale / 128.0f) * (*(Alpha *)&g->alpha / 128.0f);
                        hw = tbl[o->rect].w * 0.5f;
                        hh = tbl[o->rect].h * 0.5f;
                        x = (f32)(o->x + o->waveX + o->ofsX);
                        y = (f32)(o->y + o->waveY + o->ofsY);
                        if (o->topLeft == 0) {
                            x += hw;
                            y += hh;
                        }
                        SprPushTransform(o->angle, x, y, o->sx, o->sy);
                        SprDrawRect(&tbl[o->rect], -hw, -hh);
                        SprPopTransform();
                    }
                }
                SprFlush();
            }
            last4 = 0;
            if (flagA == 1) {
                for (j = 0; j < g->count; j++) {
                    o = &g->sprites[j];
                    if (o->active == 0) {
                        continue;
                    }
                    if (o->layer != layer) {
                        continue;
                    }
                    if (!(o->drawFlags & 2)) {
                        continue;
                    }
                    if (o->animNo == 0) {
                        continue;
                    }
                    if (o->animNo != last4) {
                        last4 = o->animNo;
                        GphAnimSetSelect(miniLayout->images[last4 + miniLayout->hdr->texCount - 1]);
                        GphTexReplace(((GphAnimSet *)miniLayout->images[last4 + miniLayout->hdr->texCount - 1])->tex, arg0);
                        GphTexBind(arg0);
                        miniUi->curTex = -1;
                    }
                    MiniSpriteDrawAnim(g, o);
                }
            }
            if (flagB == 1) {
                last = 0;
                SprBegin(g->x, g->y);
                SprSetScale(g->sx, g->sy);
                for (j = 0; j < g->count; j++) {
                    o = &g->sprites[j];
                    if (o->active == 0) {
                        continue;
                    }
                    if (o->layer != layer) {
                        continue;
                    }
                    if (o->text == NULL) {
                        continue;
                    }
                    if (!(o->drawFlags & 4)) {
                        continue;
                    }
                    if (o->text->visible == 0) {
                        continue;
                    }
                    if (last == 0) {
                        last = 1;
                        TxtWinTextureBind(0);
                        miniUi->curTex = -1;
                    }
                    a = o->a;
                    ((SprStateBits *)&sprState)->a = a * (o->alphaScale / 128.0f) * (*(Alpha *)&g->alpha / 128.0f);
                    MiniSpriteDrawText(g, o, a);
                }
                SprFlush();
            }
            last = 0;
            if (g->showNumbers != 0) {
                for (j = 0; j < g->count; j++) {
                    o = &g->sprites[j];
                    if (o->active == 0) {
                        continue;
                    }
                    if (o->layer != layer) {
                        continue;
                    }
                    if (o->num == NULL) {
                        continue;
                    }
                    if (!(o->drawFlags & 8)) {
                        continue;
                    }
                    if (o->num->visible == 0) {
                        continue;
                    }
                    if (last == 0) {
                        last = 1;
                        GphTexBind(0xEB);
                        miniUi->curTex = -1;
                    }
                    MiniSpriteDrawNumber(g, o);
                }
            }
        }
        layer++;
    } while (any == 1);
    MiniSparklesUpdate();
    MiniSparklesDraw();
}
