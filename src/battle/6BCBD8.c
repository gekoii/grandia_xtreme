#include "common.h"
#include "battle/unit.h"
#include "protos/battle.h"

s32 BtlCharDefFinish(BtlCharDef *e, s32 index) {
    s32 i;

    if (btlCharDefFinishBusy == 0) {
    if (e->id > 0x1000) {
        ChrPortraitSetByCode(e->slot, e->id - 0x60);
    }
    for (i = 0; i < 12; i++) {
        if (e->motions[i] == NULL) {
            if (i == 2) {
                e->motions[2] = e->motions[1];
                e->motionEvents[2] = e->motionEvents[1];
            } else {
                struct GphMotion *a = e->motions[0];
                s16 *b = e->motionEvents[0];
                e->motions[i] = a;
                e->motionEvents[i] = b;
            }
        }
    }
    btlCharDefFinishBusy = 0;
    return 1;
    }
    return 0;
}
