#include "common.h"
#include "field/n21.h"

void AmapOccSetMoveB(OccEntry *obj, u16 c, u16 d, u16 e, f32 x, f32 y, f32 z, u16 f, u16 k) {
    OccTween *s;
    obj->kind = 2;
    if (d) obj->flags |= 0x100;
    if (k) obj->flags |= 0x200;
    s = &obj->params.tween;
    s->target = 0;
    s->ease = 2;
    s->id = c;
    s->part = d;
    s->last = e;
    s->to.f[0] = x;
    s->to.f[1] = y;
    s->to.f[2] = z;
    s->time = f;
    s->interval = 0;
    obj->unk1E = k;
    s->accel = 0;
    s->seForward = 0;
    s->seBackward = 0;
}
