#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniSpriteSetMsgGroup(MiniSprite *objs, u8 id, u8 base) {
    u32 i;
    MiniMsgGroup *p;

    p = &miniLayout->groups[id];
    for (i = 0; i < p->count; i++) {
        MiniSpriteSetMsg(&objs[i], p->first + i, base + i);
    }
}
