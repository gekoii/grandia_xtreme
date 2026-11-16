#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

extern s32 mapPolyRejectBound;
extern s32 mapPolyRejectPlane;
extern s32 mapPolyRejectInside;
extern s32 mapPolyAcceptCount;
extern s32 mapPolyMeshCount;
extern s32 mapPolyStat54;
extern s32 mapPolyMeshNearCount;
extern s32 mapPolyStat5C;

void MapPolyAddExt(MapPolyTri *arg0) {
    MapPolyAdd(arg0);
}

void MapPolyClear(void) {
    mapPolyList.count = 0;
    mapPolyMeshCount = 0;
    mapPolyStat54 = 0;
    mapPolyMeshNearCount = 0;
    mapPolyStat5C = 0;
    mapPolyRejectBound = 0;
    mapPolyRejectPlane = 0;
    mapPolyRejectInside = 0;
    mapPolyAcceptCount = 0;
}
