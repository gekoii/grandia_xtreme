#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void BtlCaptionShow(u8 *arg0, s32 arg1, BtlUnit *chr) {
    BtlCaption *p = &btlCaption;
    s32 w, h;

    p->timer = arg1;
    p->text = arg0;
    p->state = 0;
    MenuFrameInit(&p->frame, (s16)(TxtStringWidth(0, arg0, btlCaptionTextStyle) + 0x10), 0x20);
    w = p->frame.w / 2;
    h = p->frame.h / 2;
    p->frame.x = 320.0f;
    p->frame.y = 350.0f;
    p->frame.bg.originX = w + 1;
    p->frame.bg.originY = h + 1;
    p->frame.ox = w;
    p->frame.oy = h;
    p->unit = chr;
    if (chr != NULL) {
        p->partners[0] = chr->comboPartners[0] >= 0 ? &btlUnits[chr->comboPartners[0]] : NULL;
        p->partners[1] = chr->comboPartners[1] >= 0 ? &btlUnits[chr->comboPartners[1]] : NULL;
        p->partners[2] = chr->comboPartners[2] >= 0 ? &btlUnits[chr->comboPartners[2]] : NULL;
    } else {
        p->partners[2] = NULL;
        p->partners[1] = NULL;
        p->partners[0] = NULL;
    }
    p->stamp.mode = 0;
}
