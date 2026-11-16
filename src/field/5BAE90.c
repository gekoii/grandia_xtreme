#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccPartFlipStart(OccEntry *o) {
    OccPartFlip *s = OCC_PARAMS(o, OccPartFlip);
    s32 i;
    GphPart *n;

    s->model = FldObjGet(OCC_PARAMS(o, OccPartFlip)->id);
    if (s->model == 0) {
        OccEnd(o);
        return;
    }
    s->wait = s->period + 1;
    if (o->state == 4) {
        o->timer = s->last - s->first + 1;
        s->cur = s->first;
    } else {
        o->timer = s->last - s->first - 1;
        s->cur = s->last - 1;
    }
    i = s->first;
    for (;;) {
        if (i > s->last) {
            return;
        }
        n = ChrModelFindPart(s->model, i);
        if (n == NULL) {
            break;
        }
        if (s->cur != i) {
            n->flags |= 8;
        } else {
            n->flags &= ~8;
        }
        i++;
    }
    ((s32 (*)(OccEntry *))OccEnd)(o);
}
