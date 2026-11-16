#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void BtlNoticeBrokenShow(u8 *arg0) {
    BtlNotice *p = &btlNotice;
    p->kind = 1;
    p->name = arg0;
    p->state = 0;
    MenuFrameInit(&p->frame, (s16)(TxtStringWidth(0, arg0, btlNoticeNameTextStyle) + 0x10), 0x20);
    p->frame.x = -16.0f;
    p->frame.y = 104.0f;
    p->frame.ox = 0;
    p->frame.oy = 0;
    p->frame.bg.originX = 1;
    p->frame.bg.originY = 1;
}
