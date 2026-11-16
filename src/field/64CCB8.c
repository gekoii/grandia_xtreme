#include "common.h"
#include "field/n25.h"

void MiniMsgBoxHide(s32 arg0) {
    u8 *p;
    u32 i;

    p = &((MiniSprite *)arg0)->active;
    i = 0;
    do {
        *p = 0;
        i += 1;
        p += sizeof(MiniSprite);
    } while (i < 3U);
}
