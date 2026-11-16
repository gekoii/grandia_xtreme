#include "common.h"
#include "protos/field.h"
#include "field/n17.h"


void MapCollGatherQuery(s32 arg0, f32 arg1) {
    s32 flags;
    s32 r;

    flags = MapCollGetFlags();
    MapBorderClearHits();
    MapPolyClear();
    MapWallSelectLayer(0);
    if (flags & MAP_COLL_POLYGON) {
        r = MapCollModelGet(0);
        if (r != 0) {
            FldCollModelGatherSphere(r, arg0, arg1);
        }
    }
    if (flags & MAP_COLL_HTA) {
        MapHtaGatherQuery((void *)arg0, arg1);
    }
}
