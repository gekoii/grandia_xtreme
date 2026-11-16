#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccMapJumpForward(OccEntry *obj) {
    OccMapJump *s = OCC_PARAMS(obj, OccMapJump);

    if (--s->wait <= 0) {
        obj->state = 0;
        obj->flags &= 0xFFCF;
        MapJumpRequest(OCC_PARAMS(obj, OccMapJump)->map, s->entrance);
        return;
    }
    s->frame++;
}
