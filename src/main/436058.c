/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "main/n04.h"

void GphMotKeyAlpha(GphPart *o, GphMotKeys *a, f32 t) {
    s8 *vals;
    s32 k;
    s32 k1;
    u16 *tp;
    f32 t0;
    f32 r;
    f32 v;
    f32 w;
    s8 *p0;
    s8 *p1;

    vals = a->values;
    if (a->count == 1) {
        o->color[3] = vals[0];
        return;
    }
    k = t;
    tp = a->frames;
    k = a->frameIndex[k];
    t0 = tp[k];
    if (t0 == t) {
        p0 = &vals[k];
        if ((o->gsFlags >> 11) & 3) {
            v = *p0;
            v = v + v;
            o->color[0] = v;
            o->color[1] = v;
            o->color[2] = v;
            return;
        }
        o->color[3] = *p0;
        return;
    }
    k1 = k + 1;
    p1 = &vals[k1];
    r = (t - t0) / ((f32)tp[k1] - t0);
    p0 = &vals[k];
    if ((o->gsFlags >> 11) & 3) {
        w = *p0 * (1.0f - r) + *p1 * r;
        w = w + w;
        o->color[0] = w;
        o->color[1] = w;
        o->color[2] = w;
        return;
    }
    o->color[3] = *p0 * (1.0f - r) + *p1 * r;
}
