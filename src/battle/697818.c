#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "sys.h"

f32 VuSinAngle(s32);

void BtlNoticeDraw(void) {
    BtlNotice *p = &btlNotice;
    MenuFrame *o;
    u32 c;

    if (p->name == NULL) {
        return;
    }
    if (p->state < 0x10) {
        return;
    }
    if (p->state >= 0x20) {
        return;
    }
    o = &p->frame;
    TxtGlyphBufUpload(&btlNoticeGlyphBuf);
    SprBegin(p->frame.x, o->y);
    sprState.sx = o->sx;
    sprState.col[0].a = o->alpha * 128.0f;
    if (p->kind == 0) {
        c = UtilHsvToRgb(0x9800, 0.95f, VuSinAngle(btlTickCount << 13) * 0.15f + 0.25f);
        MenuTextSetAddColor(c >> 24, c >> 16, c >> 8);
    }
    MenuTextDraw(&btlNoticeNameText);
    if (p->kind == 1) {
        MenuTextSetAddColor(0x40, 0x10, 0);
    }
    MenuTextDraw(&btlNoticeSuffixText);
    SprFlush();
}
