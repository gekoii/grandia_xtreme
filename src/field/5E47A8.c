/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n13.h"
#include "field/n17.h"

void MapBorderAddSegments(void *, void *, void *, f32);

s32 MapBorderGather(VuVec3 *arg0, f32 arg1) {
    s32 i;
    s32 n;
    MapWall *a;
    f32 y;
    f32 bottom;

    n = MapWallGetCount();
    if (n == 0) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        a = (MapWall *)MapWallGet(i);
        if (a->attr.i != 0) {
            bottom = a->bottom;
            y = arg0->y;
            if (y + 60.0f < bottom) {
                continue;
            }
            if ((f32)a->top < y) {
                continue;
            }
        }
        if (a->count != 0) {
            MapBorderAddPolygon(a, arg0, arg1);
        } else {
            MapBorderAddCircle(a, arg0, arg1);
        }
        if (mapBorderHitList->count >= 64) {
            break;
        }
    }
    return mapBorderHitList->count;
}
