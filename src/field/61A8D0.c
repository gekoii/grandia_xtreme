#include "common.h"
#include "field/n22.h"

extern void *FldMemAlloc(s32 size);
extern void MemSet(void *, s32, s32);

void MapNearObjInit(void) {
    MapNearObjTable **p = &mapNearObjTable;
    void *t;

    t = FldMemAlloc(0x610);
    *p = t;
    MemSet(t, 0, 0x610);
}
