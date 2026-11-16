#include "common.h"
#include "protos/field.h"
#include "field/n23.h"


void FldCollGather(void *arg0, f32 arg1) {
    s32 flags;
    s32 r;

    flags = MapCollGetFlags();
    fldCollTriTestCount = 0;
    fldCollTriBoundCount = 0;
    fldCollTriPlaneCount = 0;
    fldCollTriHitCount = 0;
    fldCollStat10 = 0;
    MapBorderClearHits();
    MapPolyClear();
    MapWallSelectLayer(1);
    if (flags & 2) {
        MapBorderGather(arg0, arg1);
    }
    if (flags & 8) {
        r = MapCollModelGet(1);
        if (r != 0) {
            FldCollModelGatherSphere(r, arg0, arg1);
        }
    }
    if (flags & 4) {
        MapHtaGatherQuery(arg0, arg1);
    }
}
