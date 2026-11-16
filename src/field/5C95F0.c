#include "common.h"
#include "pad.h"
#include "protos/field.h"

void FldPadApplyInputLock(Pad *o) {
    s32 i;

    if (FldInputLockGet() == 1) {
        o->held = 0;
        o->released = 0;
        o->trigger = 0;
        o->repeat = 0;
        o->prevHeld = 0;
        for (i = 0; i < 0x10; i++) {
            o->pressure[i] = 0;
            o->repeatCount[i] = 0;
        }
        o->leftX = 0x80;
        o->leftY = 0x80;
        o->rightX = 0x80;
        o->rightY = 0x80;
    }
}
