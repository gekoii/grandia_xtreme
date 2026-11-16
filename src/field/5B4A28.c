#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n12.h"


void OccMoveEnd(OccEntry *e) {
    OccTween *s = &e->params.tween;
    EvtArea *area;
    s32 i;

    if (s->target == 1) {
        for (i = 0; i <= s->last; i++) {
            area = EvtAreaGet(s->id + i);
            if (area != NULL) {
                EvtAreaClearMove(area);
            }
        }
    }
}
