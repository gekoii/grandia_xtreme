#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniSpriteSetTextSize(MiniSprite *o, u8 size, s8 b) {
    o->text->w = size;
    o->text->h = b;
    if (o->text->str == NULL) {
        o->text->width = TxtMsgGetWidth(miniLayout->msgs, o->text->msg, size);
    } else {
        o->text->width = TxtStrGetWidth(o->text->str, size);
    }
}
