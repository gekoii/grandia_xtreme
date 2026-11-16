#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void MemSet(void *, s32, s32);

void MapLoadStateInit(void) {
    MapLoadState *p = &mapLoadState;

    MemSet(p, 0, 0x30);
    MapCacheClearSmall();
    p->largeId = 0;
    p->largeSize = 0xFFFFFFFF;
    p->largeBuf = 0xFFFFFFFF;
}
