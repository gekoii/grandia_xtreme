#include "common.h"
#include "battle/n33.h"

void BtlHudStatChangeShow(BtlUnit *arg0, s32 arg1) {
    BtlHudStatChange *e;

    e = &btlHudStatChanges[arg0->id];
    if (arg1 != 0) {
        e->bits = arg1 | (arg1 >> 8);
        e->powUp = arg0->powUp;
        e->index = 0;
        e->timer = 0;
    }
}
