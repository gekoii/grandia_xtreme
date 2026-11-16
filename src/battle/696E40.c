/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"

f32 VuSinAngle(s32);

void BtlCaptionUpdate(void) {
    BtlCaption *p = &btlCaption;
    MenuFrame *o;
    BtlCaptionStamp *s;
    BtlCaptionStamp *s2;

    BtlNoticeUpdate();
    if (btlCaption.text == NULL) {
        return;
    }
    if (p->timer > 0) {
        p->timer--;
        if (p->timer <= 0) {
            p->state = 0x20;
        }
    }
    o = &p->frame;
    switch (p->state) {
    case 1:
        if (p->time != btlFrameCount) {
            TxtGlyphBufReset(&btlCaptionGlyphBuf);
            TxtObjSetString(&btlCaptionText, &btlCaptionGlyphBuf, p->text);
            p->state = 0x10;
        }
        break;
    case 0x10:
        o->alpha = 0.0f;
        p->state++;
    case 0x11:
        o->alpha += 0.125f;
        if (o->alpha >= 1.0f) {
            o->alpha = 1.0f;
            if (p->stamp.mode == 1) {
                s2 = &p->stamp;
                s2->mode = 2;
                s2->t = 0;
                s2->y = s2->vel = 0.0f;
            }
            p->state++;
        }
        o->sx = VuSinAngle(o->alpha * 16384.0f);
        break;
    case 0:
    case 0x20:
        o->alpha = 0.0f;
        p->time = btlFrameCount;
        p->state++;
        break;
    case 0x21:
        if (p->time != btlFrameCount) {
            btlCaption.text = NULL;
        }
        break;
    }
    if (p->stamp.mode >= 2) {
        s = &p->stamp;
        switch (p->stamp.mode) {
        case 2:
            if (s->t >= 0x80) {
                p->stamp.mode++;
            }
            break;
        case 3:
            s->vel -= 1.0f;
            s->y += s->vel;
            if (s->y < -414.0f) {
                p->stamp.mode = 0;
            }
            break;
        }
        s->t++;
    }
}
