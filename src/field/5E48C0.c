/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n13.h"
#include "field/n17.h"

/* Real definition is (void *, void *, s32, f32); declaring the float before the
   count gives the original's argument-load order under EABI. */
void MapBorderAddSegments(void *, void *, f32, s32);

s32 MapBorderGatherSegments(void *arg0, void *arg1, s32 arg2, f32 arg3) {
    s32 i;
    s32 n;
    MapWall *a;

    n = MapWallGetCount();
    i = 0;
    for (; i < n;) {
        a = (MapWall *)MapWallGet(i);
        if (a->count == 0) {
            MapBorderAddCircle(a, arg0, arg3);
        }
        if (mapBorderHitList->count >= 64) {
            break;
        }
        i++;
    }
    /* Dead test: both arms vanish after register allocation, but the extra use
       of arg1 is what gives it its callee-saved register. */
    if (arg1 != NULL) {
        i = 0;
        i = 0;
    } else {
        i = 0;
        i = 0;
    }
    MapBorderAddSegments(arg1, arg0, arg3, arg2);
    return 1;
}
