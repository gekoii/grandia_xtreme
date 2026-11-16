/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccPartCollapseForward(u8 *arg0) {
    OccPartCollapse *a = OCC_PARAMS(arg0, OccPartCollapse);
    f32 v[3];
    GphModel *m;
    s32 st;
    s32 i;
    s32 j;
    s32 k;
    f32 f;
    GphPart *o;

    m = FldObjGet(a->id);
    if (a->expand == 0) {
        st = ++a->frame >= a->time;
    } else {
        st = 2;
        if (--a->frame != 0) {
            st = 0;
        }
    }
    j = 0;
    for (i = a->first; i <= a->last; i++, j++) {
        switch (st) {
        case 0:
            f = 1.0f - OccEase(a->ease, (f32)a->frame / (f32)a->time);
            v[0] = a->pos[j][0] * f;
            v[1] = a->pos[j][1] * f;
            v[2] = a->pos[j][2] * f;
            break;
        case 1:
            v[0] = 0.0f;
            v[1] = 0.0f;
            v[2] = 0.0f;
            break;
        case 2:
            v[0] = a->pos[j][0];
            v[1] = a->pos[j][1];
            v[2] = a->pos[j][2];
            break;
        }
        for (k = 0; k < 3; k++) {
            if (a->jitter[k] != 0) {
                v[k] += OccRandSpread(a->jitter[k]);
            }
        }
        o = ChrModelFindPart(m, i);
        o->pos[0] = v[0];
        o->pos[1] = v[1];
        o->pos[2] = v[2];
    }
}
