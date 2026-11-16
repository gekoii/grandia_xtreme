#include "common.h"
#include "field/n21.h"

void AmapOccSetSwitchEx(u8 *arg0, u16 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6) {
    OccSwitch *s;

    if (arg3 != 0) {
        *(u16 *)(arg0 + 0x16) |= 0x100;
    }
    *(s16 *)(arg0 + 0x20) = arg2;
    *(s16 *)(arg0 + 0x18) = 5;
    s = (OccSwitch *)(arg0 + 0x20);
    s->mode = arg5;
    s->id = arg1;
    s->part = arg3;
    s->last = arg4;
    s->interval = arg6;
    s->time = 0;
    s->ramp = 0;
    s->unk10 = 0;
}
