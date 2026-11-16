#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccPartFlipUpdate(OccEntry *o) {
    OccPartFlip *s = OCC_PARAMS(o, OccPartFlip);
    s32 i;
    GphPart *n;

    if (s->wait > 0) {
        if (--s->wait != 0 || o->timer != 0) {
            o->timer++;
        }
    } else {
        s->wait = s->period;
        if (o->state == 4) {
            s->cur++;
        } else {
            s->cur--;
        }
        for (i = s->first; i <= s->last; i++) {
            n = ChrModelFindPart(s->model, i);
            if (s->cur != i) {
                n->flags |= 8;
            } else {
                n->flags &= ~8;
            }
        }
    }
}
