/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n17.h"

void VuVecCopy(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);

void *MapHtaResolve(f32 *out, void *arg1, f32 r)
{
    FVECTOR v;
    FVECTOR w;
    s32 i;
    s32 n;
    s32 hits;
    s32 rc;
    struct EvtArea *e;
    void *found;

    n = mapHtaHitList.count;
    hits = 0;
    found = NULL;
    VuVecCopy(v, out);
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(w) : "memory");
    for (i = 0; i < n; i++) {
        e = mapHtaHitList.areas[i];
        rc = MapHtaHitArea(arg1, v, e, r + 5.0f);
        if (rc != 0) {
            if (rc & 1) {
                found = e;
            }
            if (VuVecDot(v, w) > 0.0f) {
                hits++;
                if (hits >= 2) {
                    out[0] = 0;
                    out[1] = 0;
                    out[2] = 0;
                    return found;
                }
            }
            VuVecCopy(w, v);
        }
    }
    VuVecCopy(out, v);
    return found;
}
