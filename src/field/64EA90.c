#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "field/n25.h"


void MiniSpriteDrawText(void *h, MiniSprite *o, u8 a) {
    SprRect r;
    s32 buf[64];
    s8 m[2];
    u32 w;
    u32 i;
    u8 n;
    u8 *src;
    s32 u;
    s32 v;
    f32 hw;
    f32 hh;
    MiniTextSlot *t;

    w = 0;
    n = o->text->count;
    switch (o->text->mode) {
    case 0:
    default:
        src = miniLayout->msgs;
        break;
    case 1:
        src = o->text->str;
        break;
    }
    TxtMsgGetCodes(src, o->text->msg, buf);
    for (i = 0; i < n; i++) {
        TxtGlyphGetMetrics(buf[i], (u8 *)&m[0], (u8 *)&m[1], sysGrxsysPac->glyphTable);
        t = o->text;
        r.h = 0x10;
        r.w = (m[0] - m[1]) * 16.0f * 0.0625f;
        if (t->id >= 14) {
            u = i * 16 + 0xE0;
            v = (t->id - 14) * 18;
        } else if (i < 28) {
            u = i * 16;
            v = t->id * 18;
        } else {
            u = (i - 28) * 16;
            v = (t->id + 3) * 18;
        }
        r.u0 = m[1] + u;
        r.u1 = m[0] + u;
        r.v1 = r.h + v;
        r.v0 = v;
        r.w = r.w * (t->w * 0.0625f);
        hw = r.w * 0.5f;
        r.h = r.h * (t->h * 0.0625f);
        hh = r.h * 0.5f;
        SprPushTransform(o->angle,
                      (f32)o->x + o->textOfsX + o->waveX + o->ofsX + w + hw + ((w + hw) - t->width * 0.5f) * (o->sx - 1.0f),
                      (f32)o->y + o->textOfsY + o->waveY + o->ofsY + hh,
                      o->sx, o->sy);
        SprSetColor(0, 0, 0);
        SprDrawRect(&r, -hw + 2.0f, -hh + 2.0f);
        SprSetColor(0x80, 0x80, 0x80);
        SprDrawRect(&r, -hw, -hh);
        SprPopTransform();
        {
            u32 x = w + 2;
            w = x + r.w;
        }
        if (o->num != NULL && o->num->gapOn == 1 && i == o->num->gapAt) {
            w += o->num->gapWidth;
        }
    }
}
