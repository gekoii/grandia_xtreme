#include "common.h"
#include "field/event.h"

static inline void vset(f32 *d, f32 x, f32 y, f32 z) {
    d[0] = x;
    d[1] = y;
    d[2] = z;
}

void EvtAreaMove(EvtArea *o, f32 *v) {
    f32 x = v[0];
    f32 y = v[1];
    f32 z = v[2];

    vset(o->move, x, y, z);
    o->pos[0] += x;
    o->pos[1] += y;
    o->pos[2] += z;
}
