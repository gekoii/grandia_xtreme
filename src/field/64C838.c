#include "common.h"
#include "protos/field.h"
#include "field/n18.h"
#include "field/n25.h"

void MiniSpriteSetMsg(MiniSprite *o, s8 msg, u8 id) {
    MiniTextSlot *s;
    u32 i;
    u16 *str;
    TxtMsgHdr *f;

    s = &miniUi->texts[id];
    if (s->owner != NULL && o != s->owner) {
        s->owner->text = NULL;
    }
    s->msg = msg;
    s->visible = 1;
    s->id = id;
    s->owner = o;
    s->len = TxtMsgGetLength((TxtMsgHdr *)miniLayout->msgs, msg);
    if (s->len >= 29) {
        f = (TxtMsgHdr *)miniLayout->msgs;
        str = *(u16 **)(msg * 4 + (f->strOfs + (s32)f));
        for (i = 0; i < s->len; i++) {
            if (i < 28) {
                TxtGlyphQueuePush(0, str[i], i, id);
            } else {
                TxtGlyphQueuePush(0, str[i], i - 28, id + 3);
            }
        }
    } else {
        TxtGlyphQueueMsg(0, (TxtMsgHdr *)miniLayout->msgs, s->id, s->msg);
    }
    s->w = 0x16;
    s->count = s->len;
    s->h = 0x16;
    s->align = 0;
    s->mode = 0;
    s->str = NULL;
    s->width = TxtMsgGetWidth((TxtMsgHdr *)miniLayout->msgs, msg, 0x16);
    o->text = s;
    o->drawFlags |= 4;
}
