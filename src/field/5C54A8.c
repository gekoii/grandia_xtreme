/* as: ee-gcc2.9-991111-01 */
#include "common.h"

s32 ChrTenantAreaContains(f32 *pt, f32 *rc) {
    f32 x0, y0, x1, y1;
    f32 x, z;
    f32 t;
    s32 r;

    r = (s32)rc;
    x0 = ((f32 *)r)[0];
    y0 = ((f32 *)r)[1];
    x1 = ((f32 *)r)[2];
    y1 = ((f32 *)r)[3];
    if (x1 < x0) {
        t = x0;
        x0 = x1;
        x1 = t;
    }
    if (y1 < y0) {
        t = y0;
        y0 = y1;
        y1 = t;
    }
    r = (s32)pt;
    x = ((f32 *)r)[0];
    z = ((f32 *)r)[2];
    if (x < x0) {
        return 0;
    }
    if (x1 < x) {
        return 0;
    }
    if (z < y0) {
        return 0;
    }
    r = 1;
    if (y1 < z) {
        r = 0;
    }
    return r;
}
