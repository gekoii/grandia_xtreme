/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

s32 MapMiniIsExploredAt(f32 *p) {
    u8 z;
    u8 x;
    u8 row;
    s32 sel;

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
    return MapMiniExploredGet(MapMiniIndexGet(EvtGetMapNo()) + sel, row * 16 + x);
}
