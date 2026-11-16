/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"
#include "field/n17.h"


void OccMotionForward(OccEntry *o) {
    OccMotion *s = OCC_PARAMS(o, OccMotion);
    ChrObj tmp;
    ChrObj *e;
    MapModel *mm;
    GphModel *h;

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
        tmp.model = h;
        break;
    case 1:
        e = ChrTenantGet(s->id);
        if (e == NULL) {
            OccEnd(o);
            return;
        }
        {
            GphModel *t = e->model;
            if (t == NULL) {
                OccEnd(o);
                return;
            }
            h = t;
        }
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
        h = e->model;
        break;
    default:
        OccEnd(o);
        return;
    }
    if (o->flags & OCC_MOT_PLAY) {
        return;
    }
    if (ChrMotGetLastFrame(h) == 0) {
        o->timer = 0;
        return;
    }
    if (!(ChrMotFlagsGet(h) & 8)) {
        s->frame += e->motSpeed;
    }
    o->timer = 1;
    if (s->frame >= s->length) {
        s->frame = s->length;
        if (s->loops == 0) {
            s->frame -= s->length;
        } else if (s->loop < s->loops) {
            s->loop++;
            s->frame = 0.0f;
        } else if (!(o->flags & OCC_MOT_HOLD)) {
            o->timer = 0;
        } else if (!(ChrMotFlagsGet(h) & 4)) {
            o->timer = 0;
        }
    }
    ChrMotSetFrame(h, s->frame);
    if (o->timer != 0) {
        return;
    }
    switch (s->target) {
    case 0:
        h->motion[0].motion = NULL;
        break;
    case 1: {
        ChrObj *e1 = ChrTenantGet(s->id);
        h = e1->model;
        e1->flags &= ~0x40;
        break;
    }
    case 2:
    case 3: {
        ChrObj *e2;
        if (s->target == 2) {
            e2 = ChrGetById(s->id);
        } else {
            e2 = ChrGetByPartySlot(s->id);
        }
        h = e2->model;
        e2->flags &= ~0x40;
        break;
    }
    }
    ChrMotFlagsSet(h, 3, 1);
    ChrMotFlagsSet(h, 0xC, 0);
}
