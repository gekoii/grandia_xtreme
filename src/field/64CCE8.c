#include "common.h"
#include "field/n25.h"

void MiniSpriteSetNumber(MiniSprite *o, u32 value, u8 id) {
    MiniTextSlot *s;
    f32 f;

    s = &miniUi->nums[id];
    if (s->owner != NULL && o != s->owner) {
        s->owner->text = NULL;
    }
    s->visible = 1;
    s->id = id;
    s->len = 1;
    s->w = 0x18;
    s->h = 0x18;
    s->align = 0;
    s->owner = o;
    s->value = value;
    o->num = s;
    o->drawFlags |= 8;
    f = value;
    while (f >= 10.0f) {
        f *= 0.1f;
        s->len++;
    }
}
