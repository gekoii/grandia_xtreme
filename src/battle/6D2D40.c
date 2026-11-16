#include "common.h"
#include "battle/n33.h"

void BtlHudStatChangeUpdate(void) {
    BtlHudStatChange *e;
    s32 i;

    e = btlHudStatChanges;
    for (i = 0; i < 0x10; i++, e++) {
        if (e->bits == 0) {
            continue;
        }
        if (++e->timer >= 0x20) {
            e->timer = 0;
            e->index++;
        }
        while (e->index < 5 && !((e->bits >> e->index) & 1)) {
            e->timer = 0;
            e->index++;
        }
        if (e->index >= 5) {
            e->bits = 0;
        }
    }
}
