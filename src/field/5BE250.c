#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccAlphaPulseForwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccAlphaPulse *a = OCC_PARAMS(obj, OccAlphaPulse);
    s32 i;
    GphModel *m;
    s32 t;

    for (i = 0; i <= a->last; i++) {
        m = NULL;
        switch (a->target) {
        case 0:
            m = FldObjGet(a->id + i);
            break;
        case 1:
            m = ChrTenantGet(a->id + i)->model;
            break;
        case 2:
            m = ChrGetByNo(a->id + i)->model;
            break;
        }
        if (m == NULL) {
            OccEnd(obj);
            return;
        }
        t = ChrModelGetAlpha(m);
        a->start[i] = t;
        a->delta[i] = a->alpha - t;
        if (obj->flags & 0x100) {
            a->end[i] = a->alphaEnd;
        } else {
            a->end[i] = ChrModelGetAlpha(m);
        }
    }
    obj->timer = a->timeOut + (a->timeIn + a->hold);
    a->phase = 0;
    a->frame = 0;
}
