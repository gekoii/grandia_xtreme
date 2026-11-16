#include "common.h"
#include "battle/n29.h"

FxAtk *FxAtkAlloc(BtlUnit *unit, s32 flags) {
    FxAtk *p;
    s32 i;

    p = fxAtkPool;
    for (i = 32; i > 0; i--, p++) {
        if (p->flags == 0) {
            break;
        }
    }
    if (i <= 0) {
        return NULL;
    }
    p->flags = flags;
    p->unit = unit;
    p->alpha = 1.0f;
    p->shadowCb = -1;
    p->build = NULL;
    p->draw = NULL;
    p->update = NULL;
    p->work = NULL;
    p->pool = 0;
    p->buf20 = NULL;
    p->drawList = 0;
    p->dead = 0;
    p->actRequested = 0;
    return p;
}
