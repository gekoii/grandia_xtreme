#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccMoveForwardStart(OccEntry *obj) {
    OccTween *s = OCC_PARAMS(obj, OccTween);
    GphModel *e;
    GphPart *e2;
    GphLight *l;
    VuVec3 *v;
    s32 h;

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
                s->from[0] = e2->pos[0];
                s->from[1] = e2->pos[1];
                s->from[2] = e2->pos[2];
            } else {
                s->from[0] = e->pos[0];
                s->from[1] = e->pos[1];
                s->from[2] = e->pos[2];
            }
            break;
        case 1:
            h = EvtAreaGet(s->id);
            if (h == 0) {
                OccEnd(obj);
                return;
            }
            EvtAreaGetPos(h, s->from);
            break;
        case 3:
            l = FldPointLightGet(s->id);
            if (l == NULL) {
                OccEnd(obj);
                return;
            }
            s->from[0] = l->dir[0];
            s->from[1] = l->dir[1];
            s->from[2] = l->dir[2];
            break;
        case 5:
            v = FldTreasureGetModelById(s->id);
            if (v == NULL) {
                OccEnd(obj);
                return;
            }
            s->from[0] = v->x;
            s->from[1] = v->y;
            s->from[2] = v->z;
            break;
        case 2:
        case 4:
            break;
        }
        s->delta.f[0] = s->to.f[0] - s->from[0];
        s->count = 0;
        s->delta.f[1] = s->to.f[1] - s->from[1];
        s->delta.f[2] = s->to.f[2] - s->from[2];
        break;
    }
    if (s->seForward > 0) {
        FldSndSePlay(2, s->seForward, 0);
    }
}
