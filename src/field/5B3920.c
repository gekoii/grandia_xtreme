#include "common.h"
#include "field/event.h"

void EvtAreaEnable(EvtArea *e) {
    e->flags |= EVT_AREA_ENABLED;
}

void EvtAreaDisable(EvtArea *e) {
    e->flags &= ~EVT_AREA_ENABLED;
}

s32 EvtAreaIsEnabled(EvtArea *e) {
    return e->flags & EVT_AREA_ENABLED;
}
