#include "common.h"
#include "field/event.h"
#include "field/n13.h"

void OccRotateBackwardStart(OccEntry *obj) {
    OccTween *s = OCC_PARAMS(obj, OccTween);

    obj->timer = s->time;
    if (s->ease == 0) {
        s->count = s->interval;
    }
}
