#include "common.h"
#include "field/event.h"


s32 EvtAreaGetResult(s32 i) {
    EvtArea *o;

    if (i < 0) {
        return 0;
    }
    if (i >= evtAreaTable->count) {
        return 0;
    }
    o = &evtAreaTable->areas[i];
    if (!(o->flags & 1)) {
        return o->result & 0xFE;
    }
    return o->result;
}
