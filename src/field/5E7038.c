/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n17.h"

f32 MapPolyNearestVertexHeight(VuVec3 *a, MapPolyTri *p) {
    f32 dx, dz, y, vy, d, best, t, x, z, h;

    dx = a->x - p->v0[0];
    dz = a->z - p->v0[2];
    h = y = p->v0[1];
    vy = p->e1[1];
    best = dx * dx + dz * dz;
    x = p->e1[0];
    z = p->e1[2];
    t = x - dx;
    d = t * t;
    t = z - dz;
    d += t * t;
    if (d < best) {
        h = vy + y;
        best = d;
    }
    x = p->e2[0];
    z = p->e2[2];
    t = x - dx;
    d = t * t;
    t = z - dz;
    d += t * t;
    vy = p->e2[1];
    if (d < best) {
        h = vy + y;
    }
    return h;
}
