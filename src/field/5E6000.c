/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

s32 MapPolyHitTest(VuVec3 *, VuVec3 *, MapPolyTri *, f32);

s32 MapPolyGroundHeight(VuVec3 *arg0, s32 arg1, VuVec3 *arg2) {
    s32 i;
    f32 m, v;

    MapPolyList *t = &mapPolyList;
    s32 n = t->count;
    MapPolyTri *p = t->tris;
    m = 0.0f;
    p--;
    for (i = 0; i < n; i++) {
        p++;
        if (MapPolyHitTest(arg0, arg2, p, 10.0f)) {
            v = MapPolyHeightAt(arg1, p);
            m = (m < v) ? v : m;
        }
    }
    if (m > 0.01f) {
        arg2->y = m - arg0->y;
    } else {
        arg2->y = 0.0f;
    }
    return 0;
}
