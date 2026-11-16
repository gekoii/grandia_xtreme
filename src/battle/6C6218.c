/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n33.h"

s32 BtlSegIntersect(f32 *out, BtlSeg *a, BtlSeg *b) {
    f32 ax0, ay0, ax1, ay1, bx0, by0, bx1, by1;
    f32 dxa, dya, dxb, dyb;
    f32 ma, mb, ca, cb;
    f32 x, y, d;

    ax0 = a->x0; ay0 = a->z0; ax1 = a->x1; ay1 = a->z1;
    bx0 = b->x0; by0 = b->z0; bx1 = b->x1; by1 = b->z1;
    dya = a->z1 - a->z0;
    dxa = a->x1 - a->x0;
    dyb = b->z1 - b->z0;
    dxb = b->x1 - b->x0;
    ma = dya / dxa;
    mb = dyb / dxb;
    ca = a->z0 - ma * a->x0;
    cb = b->z0 - mb * b->x0;
    if (dxa == 0.0f) {
        if (dxb == 0.0f) {
            return 0;
        }
        x = a->x0;
        y = mb * x + cb;
    } else if (dxb == 0.0f) {
        x = b->x0;
        y = ma * x + ca;
    } else if (dya == 0.0f) {
        if (dyb == 0.0f) {
            return 0;
        }
        y = a->z0;
        x = (y - cb) / mb;
    } else if (dyb == 0.0f) {
        y = b->z0;
        x = (y - ca) / ma;
    } else {
        d = ma - mb;
        if (d == 0.0f) {
            return 0;
        }
        x = (cb - ca) / d;
        y = ma * x + ca;
    }
    if ((a->x0 < x && a->x1 < x) || (x < a->x0 && x < a->x1) ||
        (a->z0 < y && a->z1 < y) || (y < a->z0 && y < a->z1) ||
        (b->x0 < x && b->x1 < x) || (x < b->x0 && x < b->x1) ||
        (b->z0 < y && b->z1 < y) || (y < b->z0 && y < b->z1)) {
        return 0;
    }
    if (out != NULL) {
        out[0] = x;
        out[1] = y;
    }
    return 1;
}

