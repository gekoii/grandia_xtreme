/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


/* The two FldObjMoveTo calls before a return go through an s32 cast so they
   stay jal + return (plain calls become tail calls, 196 words off). */
void OccChrMoveForwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccChrMove *a = OCC_PARAMS(obj, OccChrMove);
    ChrObj *o0;
    ChrObj *o;
    ChrObj *p;
    ChrObj *q;
    GphModel *m;
    GphModel *m2;
    GphModel *m0;
    GphModel *pm;
    s32 fl;
    s32 i;
    s32 j;
    f32 f;
    f32 z;

    fl = 0;
    if (obj->flags & OCC_CHR_MOVE_FLAG1) {
        fl = 1;
    }
    if (obj->flags & OCC_CHR_MOVE_FLAG2) {
        fl |= 2;
    }
    if (a->speed > 0.0f) {
        obj->timer = 1;
    } else {
        obj->timer = 0;
    }
    switch (a->target) {
    case 0:
        o0 = ChrTenantGet(a->id);
        if (o0 == NULL || (m0 = o0->model) == NULL) {
            break;
        }
        if (a->mode == 1) {
            obj->timer = a->time;
            if (a->time == 0) {
                a->speed = 0.0f;
            } else {
                a->speed = 1.0f;
                f = 1.0f / a->time;
                a->vel[0] = (a->pos[0] - m0->pos[0]) * f;
                a->vel[1] = (a->pos[1] - m0->pos[1]) * f;
                a->vel[2] = (a->pos[2] - m0->pos[2]) * f;
            }
        }
        if (a->speed > 0.0f) {
            if (a->mode != 0) {
                return;
            }
            ((s32 (*)(ChrObj *, f32 *, s32, f32))FldObjMoveTo)(o0, a->pos, fl, a->speed);
            return;
        }
        m0->pos[0] = a->pos[0];
        m0->pos[1] = a->pos[1];
        m0->pos[2] = a->pos[2];
        if (obj->flags & OCC_CHR_TURN) {
            o0->moveDir = o0->heading = a->yaw * 182.04445f;
        }
        return;
    case 1:
    case 5:
        if (a->target == 1) {
            o = ChrGetById((u16)a->id);
        } else {
            o = ChrGetByPartySlot(a->id);
        }
        if (o == NULL || (m = o->model) == NULL) {
            break;
        }
        if (a->mode == 1) {
            obj->timer = a->time;
            if (a->time == 0) {
                a->speed = 0.0f;
            } else {
                a->speed = 1.0f;
                f = 1.0f / a->time;
                a->vel[0] = (a->pos[0] - m->pos[0]) * f;
                a->vel[1] = (a->pos[1] - m->pos[1]) * f;
                a->vel[2] = (a->pos[2] - m->pos[2]) * f;
            }
        }
        if (a->speed > 0.0f) {
            if (a->mode != 0) {
                return;
            }
            ((s32 (*)(ChrObj *, f32 *, s32, f32))FldObjMoveTo)(o, a->pos, fl, a->speed);
            return;
        }
        m->pos[0] = a->pos[0];
        m->pos[1] = a->pos[1];
        m->pos[2] = a->pos[2];
        if (obj->flags & OCC_CHR_TURN) {
            o->moveDir = o->heading = a->yaw * 182.04445f;
        }
        if (obj->flags & OCC_CHR_AREA) {
            o->htaArea = EvtAreaGet(a->area);
        }
        return;
    case 2:
        z = 0.0f;
        p = ChrGetByPartySlot(1);
        j = 1;
        do {
            i = j + 1;
            q = ChrGetByPartySlot(i);
            if (q != NULL && (q->flags & 1)) {
                if (a->speed > z) {
                    if (q->flags & 0x800) {
                        q->flags ^= 0x800;
                    }
                    FldObjMoveTo(q, p->model->pos, fl, a->speed);
                } else {
                    ChrPartySlotSetActive((u8)i, 0);
                    pm = p->model;
                    m2 = q->model;
                    m2->pos[0] = pm->pos[0];
                    m2->pos[1] = pm->pos[1];
                    m2->pos[2] = pm->pos[2];
                    if (obj->flags & OCC_CHR_TURN) {
                        q->moveDir = q->heading = p->heading;
                    }
                }
            }
            j = i;
        } while (j < 4);
        return;
    }
    obj->timer = 0;
}
