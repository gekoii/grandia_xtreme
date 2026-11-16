/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

void MapMiniExploreAt(f32 *p) {
    u8 z;
    u8 x;
    u8 row;
    s32 sel;
    s32 idx;

    z = (p[2] + 16000.0f) / 2000.0f;
    x = (p[0] + 16000.0f) / 2000.0f;
    if (z >= 16) {
        z = 15;
    }
    if (x >= 16) {
        x = 15;
    }
    row = 15 - z;
    sel = MapMiniFloorSelect(p[1]);
    idx = row * 16 + x;
    MapMiniExploredSet(MapMiniIndexGet(EvtGetMapNo()) + sel, idx, 1);
    if (AmapIsLoaded() == 0) {
        if (idx - 1 >= 0) {
            MapMiniExploredSet(MapMiniIndexGet(EvtGetMapNo()) + sel, idx - 1, 1);
        }
        if (idx + 1 < 15) {
            MapMiniExploredSet(MapMiniIndexGet(EvtGetMapNo()) + sel, idx + 1, 1);
        }
        if (idx + 16 < 255) {
            MapMiniExploredSet(MapMiniIndexGet(EvtGetMapNo()) + sel, idx + 17, 1);
        }
        if (idx - 16 < 0) {
            MapMiniExploredSet(MapMiniIndexGet(EvtGetMapNo()) + sel, idx - 15, 1);
        }
    }
}
