/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n17.h"

f32 MapPolyHeightAt(VuVec3 *a, MapPolyTri *p) {
    f32 ny, an, dz, z, dx, lo, hi, nx, nz, y, h, mx, mn;

    ny = p->normal[1];
    dx = a->x - p->v0[0];
    lo = p->e1[1];
    an = __builtin_fabsf(ny);
    hi = p->e2[1];
    nx = p->normal[0];
    nz = p->normal[2];
    y = p->v0[1];
    dz = a->z - p->v0[2];

    if (an >= 0.7071068f) {
        h = -(nx * dx + nz * dz) / ny;
        return h + y;
    }
    z = 0.0f;
    if (an < 0.17f) {
        h = z;
        if (z < lo) {
            h = lo;
        }
        if (h < hi) {
            h = hi;
        }
        return h + y;
    }
    h = -(nx * dx + nz * dz) / ny;
    mx = z;
    if (z < lo) {
        mx = lo;
    }
    if (mx < hi) {
        mx = hi;
    }
    if (mx < h) {
        return mx + y;
    }
    mn = 0.0f;
    if (lo < 0.0f) {
        mn = lo;
    }
    if (hi < mn) {
        mn = hi;
    }
    if (h < mn) {
        return mn + y;
    }
    return h + y;
}
