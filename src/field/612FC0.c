#include "common.h"
#include "field/n21.h"

void AmapOccSetSwitch(u8 *arg0, u16 arg1, u8 arg2, u8 arg3, u8 arg4) {
    OccSwitch *s;

    *(s16 *)(arg0 + 0x20) = arg2;
    *(s16 *)(arg0 + 0x18) = 5;
    s = (OccSwitch *)(arg0 + 0x20);
    s->mode = arg3;
    s->id = arg1;
    s->interval = arg4;
    s->part = 0;
    s->last = 0;
    s->time = 0;
    s->ramp = 0;
    s->unk10 = 0;
}
