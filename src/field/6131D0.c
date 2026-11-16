#include "common.h"
#include "field/n21.h"

void AmapOccSetUvScroll(u8 *arg0, u16 arg1, u16 arg2, s32 arg3, s32 arg4, s16 arg5) {
    OccTween *s;

    *(s16 *)(arg0 + 0x20) = 0;
    *(s16 *)(arg0 + 0x18) = 0x18;
    s = (OccTween *)(arg0 + 0x20);
    s->ease = 0;
    s->part = arg2;
    s->id = arg1;
    s->last = arg2;
    s->to.i[0] = arg3;
    s->to.i[1] = arg4;
    s->time = arg5;
    s->interval = 0;
    *(s16 *)(arg0 + 0x1E) = 0;
    s->accel = 0;
    s->seForward = 0;
    s->seBackward = 0;
}
