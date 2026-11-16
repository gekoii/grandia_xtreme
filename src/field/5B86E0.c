#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccScaleForwardStart(OccEntry *obj) {
    OccTween *s = OCC_PARAMS(obj, OccTween);
    GphModel *e;
    GphPart *e2;
    EvtArea *e3;

    obj->timer = s->time;
    switch (s->ease) {
    case 0:
        s->delta.f[0] = s->to.f[0];
        s->delta.f[1] = s->to.f[1];
        s->delta.f[2] = s->to.f[2];
        s->count = s->interval;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        switch (OCC_PARAMS(obj, OccTween)->target) {
        case 0:
            e = FldObjGet(s->id);
            if (e == NULL) {
                OccEnd(obj);
                return;
            }
            if (obj->flags & OCC_PART) {
                e2 = ChrModelFindPart(e, s->part);
                if (e2 == NULL) {
                    OccEnd(obj);
                    return;
                }
                s->from[0] = e2->scale[0];
                s->from[1] = e2->scale[1];
                s->from[2] = e2->scale[2];
            } else {
                s->from[0] = e->scale[0];
                s->from[1] = e->scale[1];
                s->from[2] = e->scale[2];
            }
            break;
        case 1:
            e3 = EvtAreaGet(s->id);
            e3->flags |= 8;
            if (e3 == NULL) {
                OccEnd(obj);
                return;
            }
            s->from[0] = e3->scale[0];
            s->from[1] = e3->scale[1];
            s->from[2] = e3->scale[2];
            break;
        case 3:
            e = FldSkyGetModel(0);
            if (e == NULL) {
                OccEnd(obj);
                return;
            }
            if (obj->flags & OCC_PART) {
                e2 = ChrModelFindPart(e, s->part);
                if (e2 == NULL) {
                    OccEnd(obj);
                    return;
                }
                s->from[0] = e2->scale[0];
                s->from[1] = e2->scale[1];
                s->from[2] = e2->scale[2];
            } else {
                s->from[0] = e->scale[0];
                s->from[1] = e->scale[1];
                s->from[2] = e->scale[2];
            }
            break;
        }
        s->delta.f[0] = s->to.f[0] - s->from[0];
        s->count = 0;
        s->delta.f[1] = s->to.f[1] - s->from[1];
        s->delta.f[2] = s->to.f[2] - s->from[2];
        break;
    }
}
