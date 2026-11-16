#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccPartCollapseBackward(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccPartCollapse *a = OCC_PARAMS(obj, OccPartCollapse);
    VuVec3 v;
    GphModel *m;
    s32 i;
    s32 j;
    GphPart *o;

    if (obj->timer <= 0) {
        m = FldObjGet(a->id);
        j = 0;
        for (i = a->first; i <= a->last; i++, j++) {
            if (a->expand == 0) {
                v.x = a->pos[j][0];
                v.y = a->pos[j][1];
                v.z = a->pos[j][2];
            } else {
                v.x = 0.0f;
                v.y = 0.0f;
                v.z = 0.0f;
            }
            o = ChrModelFindPart(m, i);
            o->pos[0] = v.x;
            o->pos[1] = v.y;
            o->pos[2] = v.z;
        }
    }
}
