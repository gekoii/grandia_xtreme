#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"
#include "field/n17.h"


void OccMotionForwardStart(OccEntry *o) {
    OccMotion *s = OCC_PARAMS(o, OccMotion);
    ChrObj tmp;
    ChrObj *e;
    MapModel *mm;
    GphModel *h;
    f32 f;
    s16 a;

    switch (OCC_PARAMS(o, OccMotion)->target) {
    case 0:
        mm = MapModelGet(s->id);
        h = FldObjGet(s->id);
        if (mm == NULL) {
            OccEnd(o);
            return;
        }
        tmp.motTable = &mm->motTable;
        e = &tmp;
        tmp.motSpeed = 1.0f;
        tmp.model = (GphModel *)h;
        break;
    case 1:
        e = ChrTenantGet(s->id);
        if (e == NULL) {
            OccEnd(o);
            return;
        }
        if (e->model == 0) {
            OccEnd(o);
            return;
        }
        h = (s32)e->model;
        e->flags |= 0x40;
        break;
    case 2:
    case 3:
        if (s->target == 2) {
            e = ChrGetById(s->id);
        } else {
            e = ChrGetByPartySlot(s->id);
        }
        if (e == NULL) {
            OccEnd(o);
            return;
        }
        h = (s32)e->model;
        e->flags |= 0x40;
        break;
    default:
        OccEnd(o);
        return;
    }
    if (o->flags & OCC_MOT_PLAY) {
        a = s->mot;
        e->mot = a;
        if (s->blend != 0) {
            ChrMotSet(e, a);
        } else {
            ChrMotSetImmediate(e, a);
        }
        ChrMotFlagsSet(e->model, 1, 1);
        if (o->flags & OCC_MOT_HOLD) {
            ChrMotFlagsSet(e->model, 4, 1);
            ChrMotFlagsSet(e->model, 2, 0);
        } else {
            ChrMotFlagsSet(e->model, 4, 0);
            ChrMotFlagsSet(e->model, 2, 1);
        }
        f = ChrMotGetLastFrame(h);
        s->length = f;
        if (o->flags & OCC_MOT_REVERSE) {
            s->frame = f;
            ChrMotFlagsSet(e->model, 4, 1);
            ChrMotFlagsSet(e->model, 2, 0);
        } else {
            s->frame = 0.0f;
        }
        if (s->loops != 0) {
            o->timer = (s->length - 2.0f) * s->loops;
        } else {
            o->timer = s->length;
        }
        o->timer = o->timer * e->motSpeed;
    } else {
        a = s->mot;
        e->mot = a;
        if (s->blend != 0) {
            ChrMotSet(e, a);
        } else {
            ChrMotSetImmediate(e, a);
        }
        ChrMotFlagsSet(e->model, 1, 0);
        if (o->flags & OCC_MOT_HOLD) {
            ChrMotFlagsSet(e->model, 2, 0);
            ChrMotFlagsSet(e->model, 4, 1);
        } else {
            ChrMotFlagsSet(e->model, 2, 1);
            ChrMotFlagsSet(e->model, 4, 0);
        }
        f = ChrMotGetLastFrame(h);
        s->length = f;
        if (o->flags & OCC_MOT_REVERSE) {
            s->frame = f;
            ChrMotFlagsSet(e->model, 4, 1);
        } else {
            s->frame = 0.0f;
        }
        s->model = h;
        s->loop = 1;
        o->timer = 1;
    }
}
