#include "common.h"
#include "field/n17.h"

extern void *FldMemAlloc(s32);
extern void MemSet(void *, s32, s32);

void MapPolyInit(void) {
    MapPolyList *l = &mapPolyList;
    l->count = 0;
    l->tris = FldMemAlloc(256 * sizeof(MapPolyTri));
    MemSet(&mapCollModel, 0, 8); /* mapCollModel and mapCamCollModel */
}
