#include "common.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"


void OccAlphaForwardStart(OccEntry *obj) {
    OccTween *s;
    f32 val;
    void *h;
    ChrObj *e;

    val = -1.0f;
    s = OCC_PARAMS(obj, OccTween);
    switch (OCC_PARAMS(obj, OccTween)->target) {
    case 0:
        if (obj->flags & 0x800) {
            h = ChrModelFindPart(FldObjGet(s->id), s->part);
            if (h != NULL) {
                val = ChrPartGetAlphaRaw(h);
            }
        } else {
            h = FldObjGet(s->id);
            if (h != NULL) {
                val = ChrModelGetAlphaRaw(h);
            }
        }
        break;
    case 1:
        e = ChrTenantGet(s->id);
        if (e != NULL) {
            h = e->model;
            if (h != NULL) {
                val = ChrModelGetAlphaRaw(h);
            }
        }
        break;
    case 2:
    case 4:
        if (s->target == 2) {
            e = ChrGetById(s->id);
        } else {
            e = ChrGetByPartySlot(s->id);
        }
        if (e != NULL) {
            h = e->model;
            if (h != NULL) {
                val = ChrModelGetAlphaRaw(h);
            }
        }
        break;
    }
    switch (s->ease) {
    case 0:
        s->delta.i[1] = 0;
        s->delta.f[0] = s->to.f[0];
        s->delta.i[2] = 0;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        s->from[0] = val;
        s->count = 0;
        s->delta.f[0] = s->to.f[0] - val;
        break;
    }
    obj->timer = s->time;
}
