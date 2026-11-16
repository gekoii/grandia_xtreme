#include "common.h"
#include "gph.h"
#include "protos/field.h"

void MapPolyGatherPart(GphPart *, s32, s32, f32);

s32 MapPolyGatherModel(GphModel *arg0, s32 arg1, s32 arg2, f32 arg3) {
    s32 i;
    GphPart *p;

    i = 0;
    MapPolyClear();
    p = arg0->parts;
    for (; i < arg0->partCount; i++) {
        if (p[i].gaObjCount != 0) {
            MapPolyGatherPart(&p[i], arg1, arg2, arg3);
        }
    }
    return 1;
}
