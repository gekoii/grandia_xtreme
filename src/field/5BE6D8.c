#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccPartCollapseForwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccPartCollapse *a = OCC_PARAMS(obj, OccPartCollapse);
    VuVec3 v;
    GphModel *m;
    s32 i;
    s32 j;
    GphPart *o;

    obj->timer = a->time;
    if (a->expand == 0) {
        a->frame = 0;
    } else {
        a->frame = a->time;
    }
    m = FldObjGet(a->id);
    i = a->last - a->first;
    if (m == 0 || i >= 12) {
        OccEnd(obj);
        return;
    }
    j = 0;
    for (i = a->first; i <= a->last; i++, j++) {
        o = ChrModelFindPart(m, i);
        if (o == NULL) {
            OccEnd(obj);
            return;
        }
        v.x = o->pos[0];
        a->pos[j][0] = v.x;
        v.y = o->pos[1];
        a->pos[j][1] = v.y;
        v.z = o->pos[2];
        a->pos[j][2] = v.z;
    }
}
