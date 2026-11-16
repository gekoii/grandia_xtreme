#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n12.h"


void OccPathEnd(OccEntry *e) {
    OccPath *s = &e->params.path;
    EvtArea *area;

    if (s->target == 1) {
        area = EvtAreaGet(s->id);
        if (area != NULL) {
            EvtAreaClearMove(area);
        }
    }
}
