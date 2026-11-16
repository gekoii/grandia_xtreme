/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n13.h"
#include "field/n17.h"


s32 MapBorderSlide(VuVec3 *arg0, s32 arg1, f32 arg2) {
    s32 i;
    s32 n;
    s32 hits;
    s32 same;
    s32 r;
    f32 px;
    f32 pz;
    f32 x;
    f32 z;
    MapBorderHit *e;
    MapWall *s;

    if (mapBorderHitList->count == 0 || (arg0->x == 0.0f && arg0->z == 0.0f)) {
        return 0;
    }
    hits = 0;
    pz = 0.0f;
    i = 0;
    n = mapBorderHitList->count;
    same = 0;
    px = 0.0f;
    for (; i < n; i++) {
        e = &mapBorderHitList->hits[i];
        if (e->type != 0) {
            r = MapBorderSlideWall(&e->wall, arg0, arg1, arg2);
        } else {
            s = e->shape;
            r = MapCollPushCircle(&s->cx, (f32 *)arg0, (f32 *)arg1, s->radius, arg2);
        }
        if (r != 0) {
            x = arg0->x;
            z = arg0->z;
            hits++;
            if (px * x + pz * z > 0.0f) {
                same++;
            }
            px = x;
            pz = z;
        }
    }
    r = 0;
    if (hits > 0) {
        r = 1;
        if (same >= 3) {
            arg0->x = 0.0f;
            arg0->z = 0.0f;
        }
    }
    return r;
}
