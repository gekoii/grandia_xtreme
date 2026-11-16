#include "common.h"
#include "field/n25.h"

void MiniMsgBoxShowLines(MiniSprite *objs, u8 id) {
    u32 i;
    MiniMsgGroup *p;

    p = &miniLayout->groups[id];
    for (i = 0; i < 3; i++) {
        if (i < p->count) {
            objs[i].active = 1;
        } else {
            objs[i].active = 0;
        }
    }
}
