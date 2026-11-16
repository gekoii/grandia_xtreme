#include "common.h"
#include "battle/n33.h"

void BtlHudStatusIconShow(s32 *arg0, s32 arg1) {
    BtlHudStatusIcon *e;

    arg1 &= ~0x2000;
    e = &btlHudStatusIcons[*arg0];
    if (arg1 != 0) {
        e->flags = arg1;
        e->index = 0;
        e->timer = 0;
    }
}
