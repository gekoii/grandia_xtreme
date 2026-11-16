/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"

extern u8 btlStrHasBroken[];
extern u8 btlStrWasLearned[];
f32 VuSinAngle(s32);

void BtlNoticeUpdate(void) {
    BtlNotice *p = &btlNotice;
    MenuFrame *o;
    f32 f;

    if (p->name == NULL) {
        return;
    }
    o = &p->frame;
    switch (p->state) {
    case 0:
    case 0x20:
        o->alpha = 0.0f;
        p->time = btlFrameCount;
        p->state++;
        break;
    case 1:
        if (p->time != btlFrameCount) {
            TxtGlyphBufReset(&btlNoticeGlyphBuf);
            TxtObjSetString(&btlNoticeNameText, &btlNoticeGlyphBuf, p->name);
            if (p->kind == 1) {
                TxtObjSetString(&btlNoticeSuffixText, &btlNoticeGlyphBuf, btlStrHasBroken);
            } else {
                TxtObjSetString(&btlNoticeSuffixText, &btlNoticeGlyphBuf, btlStrWasLearned);
            }
            p->state = 0x10;
            btlNoticeNameText.x = -btlNoticeSuffixText.width - 4;
        }
        break;
    case 0x10:
        o->alpha = 0.0f;
        p->state++;
        p->timer = 0;
    case 0x11:
        o->alpha += 0.125f;
        if (o->alpha >= 1.0f) {
            o->alpha = 1.0f;
            p->state++;
        }
    case 0x12:
        if (p->timer >= 0xB4) {
            p->timer = 0;
            p->state++;
            break;
        }
        if (p->timer < 0x10) {
            f = 1.0f - VuSinAngle(p->timer << 10);
            p->frame.x = 624.0f - f * 640.0f * f;
        }
        break;
    case 0x13:
        if (p->timer < 0x10) {
            f = VuSinAngle(p->timer << 10);
            p->frame.x = f * 640.0f * f + 624.0f;
        } else {
            p->state = 0x20;
        }
        break;
    case 0x21:
        if (p->time != btlFrameCount) {
            p->name = NULL;
        }
        break;
    }
    p->timer++;
}
