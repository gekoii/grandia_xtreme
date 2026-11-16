#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccRotateForwardStart(OccEntry *obj) {
    OccTween *s = OCC_PARAMS(obj, OccTween);
    GphModel *e;
    GphPart *e2;
    EvtArea *e3;
    ChrObj *m;

    obj->timer = s->time;
    switch (s->ease) {
    case 0:
        s->delta.i[0] = s->to.i[0];
        s->delta.i[1] = s->to.i[1];
        s->delta.i[2] = s->to.i[2];
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
                s->from[0] = e2->rot[0] * 0.0054931640625f;
                s->from[1] = e2->rot[1] * 0.0054931640625f;
                s->from[2] = e2->rot[2] * 0.0054931640625f;
            } else {
                s->from[0] = e->rot[0] * 0.0054931640625f;
                s->from[1] = e->rot[1] * 0.0054931640625f;
                s->from[2] = e->rot[2] * 0.0054931640625f;
            }
            break;
        case 1:
            e3 = EvtAreaGet(s->id);
            if (e3 == NULL) {
                OccEnd(obj);
                return;
            }
            s->from[0] = e3->rot[0] * 0.0054931640625f;
            s->from[1] = e3->rot[1] * 0.0054931640625f;
            s->from[2] = e3->rot[2] * 0.0054931640625f;
            break;
        case 2:
            m = ChrTenantGet(s->id);
            if (m == NULL) {
                OccEnd(obj);
                return;
            }
            s->from[1] = m->heading * 0.0054931640625f;
            s->from[0] = m->model->rot[0] * 0.0054931640625f;
            s->from[2] = m->model->rot[2] * 0.0054931640625f;
            break;
        case 3:
        case 4:
            if (s->target == 3) {
                m = ChrGetById(s->id);
            } else {
                m = ChrGetByPartySlot(s->id);
            }
            if (m == NULL) {
                OccEnd(obj);
                return;
            }
            s->from[1] = m->heading * 0.0054931640625f;
            s->from[0] = m->model->rot[0] * 0.0054931640625f;
            s->from[2] = m->model->rot[2] * 0.0054931640625f;
            break;
        }
        OccWrapDegrees3(s->from);
        s->delta.f[0] = s->to.f[0] - s->from[0];
        s->count = 0;
        s->delta.f[1] = s->to.f[1] - s->from[1];
        s->delta.f[2] = s->to.f[2] - s->from[2];
        break;
    }
}
