#include "common.h"
#include "field/n21.h"

void AmapOccSetRotate(u8 *arg0, u16 arg1, u16 arg2, u16 arg3, f32 arg4, f32 arg5, f32 arg6, u16 arg7, u16 arg8) {
    OccTween *s;

    *(s16 *)(arg0 + 0x18) = 3;
    if (arg2 != 0) {
        *(u16 *)(arg0 + 0x16) |= 0x100;
    }
    if (arg8 != 0) {
        *(u16 *)(arg0 + 0x16) |= 0x200;
    }
    s = (OccTween *)(arg0 + 0x20);
    *(s16 *)(arg0 + 0x20) = 0;
    s->id = arg1;
    s->ease = 2;
    s->part = arg2;
    s->last = arg3;
    s->to.f[0] = arg4;
    s->to.f[1] = arg5;
    s->to.f[2] = arg6;
    s->time = arg7;
    s->interval = 0;
    *(s16 *)(arg0 + 0x1E) = 0;
    s->accel = 0;
    s->seForward = 0;
    s->seBackward = 0;
}
