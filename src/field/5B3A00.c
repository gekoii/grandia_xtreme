#include "common.h"
#include "field/event.h"

static inline void clr(f32 *v) {
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 0.0f;
}

void EvtAreaClearMove(EvtArea *e) {
    clr(e->move);
}
