#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccScaleBackwardStart(OccEntry *obj) {
    OccTween *s = OCC_PARAMS(obj, OccTween);

    if (FldObjGet(s->id) == 0) {
        obj->state = 0;
        obj->flags &= 0xFFCF;
    } else {
        obj->timer = s->time;
        if (s->ease == 0) {
            s->count = s->interval;
        }
    }
}
