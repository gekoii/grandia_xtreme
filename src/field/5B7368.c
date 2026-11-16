#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccMoveBackwardStart(OccEntry *obj) {
    OccTween *s = OCC_PARAMS(obj, OccTween);

    obj->timer = s->time;
    if (s->ease == 0) {
        s->count = s->interval;
    }
    if (s->seBackward > 0) {
        FldSndSePlay(2, s->seBackward, 0);
    }
}
