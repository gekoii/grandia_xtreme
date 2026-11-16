#include "common.h"
#include "field/n22.h"

s32 MapNearObjGetState(u8 id) {
    MapNearObj *e = mapNearObjTable->objs;
    u32 i;

    for (i = 0; i < mapNearObjTable->count; i++, e++) {
        if (e->id == id) {
            return e->state;
        }
    }
    return -1;
}
