#include "common.h"
#include "battle/n33.h"

void BtlHudStatusIconUpdate(void) {
    BtlHudStatusIcon *e;
    s32 i;

    e = btlHudStatusIcons;
    for (i = 0; i < 0x10; i++, e++) {
        if (e->flags == 0) {
            continue;
        }
        if (++e->timer >= 0x20) {
            e->timer = 0;
            e->index++;
        }
        while (e->index < 0xC && !(e->flags & (0x10 << e->index))) {
            e->timer = 0;
            e->index++;
        }
        if (e->index >= 0xC) {
            e->flags = 0;
        }
    }
}
