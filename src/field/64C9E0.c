#include "common.h"
#include "field/n25.h"
#include "protos/field.h"

void MiniSpriteSetText(MiniSprite *o, u8 *str, u8 id, u8 mode) {
    MiniTextSlot *s;
    u8 n;

    s = &miniUi->texts[id];
    if (s->owner != NULL && o != s->owner) {
        s->owner->text = NULL;
    }
    s->owner = o;
    s->visible = 1;
    s->msg = -1;
    s->id = id;
    /* u8 return cast: the caller's register allocation needs the narrowed result (the callee returns s32 for its other callers) */
    n = ((u8 (*)(u8, u8 *, s32))TxtGlyphQueueStr)(0, str, id);
    s->mode = mode;
    s->str = str;
    s->align = 0;
    s->len = n;
    s->w = 0x16;
    s->h = 0x16;
    s->count = n;
    s->width = TxtStrGetWidth(str, 0x16);
    o->text = s;
    o->drawFlags |= 4;
}
