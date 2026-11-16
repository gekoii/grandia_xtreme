#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"
#include "field/n14.h"

void EvtAreaCalcMatrix(EvtArea *);

void OccPathForward(OccEntry *o) {
    OccPath *s = OCC_PARAMS(o, OccPath);
    VuVec3 v;
    GphModel *e;
    EvtArea *ar;
    GphModel *c;
    ChrObj *m;
    FldPath *q;
    FldPathEvent *ev;
    FldPathEvent *base;
    s32 i;

    if (s->big == 0) {
        v.x = mapPathWork->small[s->slot].x[s->frame];
        v.y = mapPathWork->small[s->slot].y[s->frame];
        v.z = mapPathWork->small[s->slot].z[s->frame];
    } else {
        v.x = mapPathWork->big[s->slot].x[s->frame];
        v.y = mapPathWork->big[s->slot].y[s->frame];
        v.z = mapPathWork->big[s->slot].z[s->frame];
    }
    switch (s->target) {
    case 0:
        e = FldObjGet(s->id);
        if (e != NULL) {
            if (!(o->flags & OCC_PATH_PLACE) && s->frame > 0) {
                e->rot[1] = FldCalcYaw(e->pos, &v);
            }
            e->pos[0] = v.x;
            e->pos[1] = v.y;
            e->pos[2] = v.z;
        }
        break;
    case 1:
        ar = EvtAreaGet(s->id);
        if (ar != NULL) {
            EvtAreaSetPos(ar, &v);
            EvtAreaCalcMatrix(ar);
        }
        break;
    case 2:
        break;
    case 3: {
        GphLight *t = FldPointLightGet(s->id);
        if (t != NULL) {
            t->dir[0] = v.x;
            t->dir[1] = v.y;
            t->dir[2] = v.z;
        }
        break;
    }
    case 4:
        m = ChrTenantGet(s->id);
        if (m != NULL && (c = m->model) != NULL) {
            if (o->flags & OCC_PATH_PLACE) {
                c->pos[0] = v.x;
                c->pos[1] = v.y;
                c->pos[2] = v.z;
            } else if (s->frame != 0) {
                FldObjMoveTo(m, &v, 0, 8000.0f);
            } else {
                c->pos[0] = v.x;
                c->pos[1] = v.y;
                c->pos[2] = v.z;
            }
        }
        break;
    case 5:
    case 6:
        if (s->target == 5) {
            m = ChrGetById(s->id);
        } else if (s->target == 6) {
            m = ChrGetByPartySlot(s->id);
        } else {
            return;
        }
        if (m != NULL && (c = m->model) != NULL) {
            if (o->flags & OCC_PATH_PLACE) {
                c->pos[0] = v.x;
                c->pos[1] = v.y;
                c->pos[2] = v.z;
            } else if (s->frame == 0) {
                c->pos[0] = v.x;
                c->pos[1] = v.y;
                c->pos[2] = v.z;
            } else {
                FldObjMoveTo(m, &v, 0, 8000.0f);
            }
        }
        break;
    }
    q = &fldPathTable[s->path];
    if (q->eventCount > 0) {
        base = &fldPathEventData[q->eventStart];
        for (i = 0; i < q->eventCount; i++) {
            ev = &base[i];
            if (ev->frame == s->frame) {
                switch (ev->type) {
                case 0:
                    if (ev->arg >= s->wait) {
                        s->frame--;
                        s->wait++;
                        o->timer++;
                    } else {
                        s->wait = 0;
                    }
                    break;
                case 1:
                    OccWake(ev->arg);
                    break;
                case 2:
                    OccsWake(ev->arg);
                    break;
                }
                break;
            }
        }
    }
    s->frame++;
    if (o->timer == 0) {
        if (s->big == 0) {
            mapPathWork->smallUsed[s->slot] = 0;
        } else {
            mapPathWork->bigUsed[s->slot] = 0;
        }
    }
}
