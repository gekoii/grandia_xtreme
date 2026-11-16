#include "common.h"
#include "field/event.h"

void EvtAreaGetPos(EvtArea *e, f32 *out) {
    out[0] = e->pos[0];
    out[1] = e->pos[1];
    out[2] = e->pos[2];
}
