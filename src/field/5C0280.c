#include "common.h"
#include "field/n13.h"
#include "protos/field.h"

void *FldMemAlloc(s32);

s32 MapWallInit(s32 arg0) {
    MapWallList **q1 = &mapWallListMove;
    MapWallList **q2 = &mapWallListCam;
    MapWallList *p;

    p = FldMemAlloc(0x1004);
    *q1 = p;
    p->count = 0;
    p = FldMemAlloc(0x1004);
    *q2 = p;
    p->count = 0;
    MapWallSelectLayer(0);
    return 0;
}
