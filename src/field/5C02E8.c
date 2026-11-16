#include "common.h"
#include "field/n13.h"

void MapWallAdd(s32 arg0) {
    MapWallList *l = mapWallList;
    l->items[l->count++] = (MapWall *)arg0;
}
