#include "common.h"
#include "field/n22.h"

void MapNearObjSetState(u8 id, u8 val) {
    u32 i;
    MapNearObj *e;

    i = 0;
    while (i < mapNearObjTable->count) {
        e = &mapNearObjTable->objs[i];
        if (e->id == id) {
            e->state = val;
            break;
        }
        i++;
    }
}
