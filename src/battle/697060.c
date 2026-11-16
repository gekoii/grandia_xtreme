#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "sys.h"

extern SprRect btlCaptionStampSprite;

void BtlCaptionDraw(void) {
    BtlCaption *p = &btlCaption;
    MenuFrame *o;
    SprState *q;
    BtlCaptionStamp *s;
    SprRect r;

    if (btlHudHideFlags & 0x10) {
        return;
    }
    BtlNoticeDraw();
    if (btlCaption.text == NULL) {
        return;
    }
    if (p->state < 0x10) {
        return;
    }
    if (p->state >= 0x20) {
        return;
    }
    o = &p->frame;
    q = &sprState;
    MenuFrameDraw(o);
    TxtGlyphBufUpload(&btlCaptionGlyphBuf);
    SprBegin(p->frame.x, o->y);
    sprState.sx = o->sx;
    q->col[0].a = o->alpha * 128.0f;
    MenuTextDraw(&btlCaptionText);
    if (p->stamp.mode >= 2) {
        s = &p->stamp;
        SprFlush();
        GphTexBind(0xE9);
        q->offX = o->w / 2;
        q->offY = (f32)(o->h / 2) + s->y;
        if (s->t < 0x1B) {
            r = btlCaptionStampSprite;
            r.h = s->t;
            r.v1 = btlCaptionStampSprite.v1 + (s->t - 0x1B);
            SprDrawRect(&r, -r.w / 2, btlCaptionStampSprite.h / 2 - s->t);
        } else {
            SprDrawRect(&btlCaptionStampSprite, -btlCaptionStampSprite.w / 2, -btlCaptionStampSprite.h / 2);
        }
        sprState.offY = 0;
        sprState.offX = 0;
    }
    if (p->unit != NULL && p->state >= 0x12) {
        SprFlush();
        GphTexBind(0xEA);
        BtlUnitIconDraw(p->unit, -o->w / 2 - 8, -o->h / 2 - 4, 0x3000);
        if (p->partners[0] != NULL) {
            if (p->partners[1] != NULL) {
                BtlUnitIconDraw(p->partners[0], o->w / 2 + 8, -o->h / 2 - 4, -0x3000);
                if (p->partners[2] != NULL) {
                    BtlUnitIconDraw(p->partners[1], -o->w / 2 - 8, o->h / 2 + 4, 0x5000);
                    BtlUnitIconDraw(p->partners[2], o->w / 2 + 8, o->h / 2 + 4, -0x5000);
                } else {
                    BtlUnitIconDraw(p->partners[1], 0, o->h / 2 + 12, 0x3000);
                }
            } else {
                BtlUnitIconDraw(p->partners[0], o->w / 2 + 8, o->h / 2 + 4, -0x5000);
            }
        }
    }
    SprFlush();
}
