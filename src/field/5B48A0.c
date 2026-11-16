#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n12.h"


s32 OccMapJumpRequest(s32 id, s32 mapNo, s32 entrance) {
    OccEntry *o;
    OccMapJump *args;

    if (id == 0) {
        MapJumpRequest(mapNo, entrance);
        return -1;
    }
    o = OccFindById(id);
    if (o->kind != 1) {
        return -1;
    }
    args = &o->params.mapJump;
    args->entrance = entrance;
    args->map = mapNo;
    ((void (*)(s16, OccEntry *))OccWakeById)(id, o);
    return 0;
}
