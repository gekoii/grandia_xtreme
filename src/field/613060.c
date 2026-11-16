#include "common.h"
#include "field/n21.h"

void AmapOccSetPartFade(u8 *arg0, u16 arg1, u16 arg2, u16 arg3, s16 arg4, s16 arg5) {
    OccTween *s;
    f32 f = arg4;

    *(s16 *)(arg0 + 0x18) = 6;
    s = (OccTween *)(arg0 + 0x20);
    *(u16 *)(arg0 + 0x16) |= 0x800;
    *(s16 *)(arg0 + 0x20) = 0;
    s->id = arg1;
    s->part = arg2;
    s->last = arg3;
    s->to.f[0] = f;
    s->time = arg5;
    s->ease = 0;
    s->interval = 0;
    *(s16 *)(arg0 + 0x1E) = 0;
    s->accel = 0;
    s->seForward = 0;
    s->seBackward = 0;
}
