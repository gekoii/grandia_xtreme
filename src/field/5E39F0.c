#include "common.h"
#include "field/n17.h"
#include "protos/field.h"

extern void *FldMemAlloc(s32);

void MapBorderInit(void) {
    MapBorderHitList **p = &mapBorderHitList;
    *p = FldMemAlloc(sizeof(MapBorderHitList));
    MapBorderClearHits();
}
