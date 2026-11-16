/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccChrMoveForward(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccChrMove *a = OCC_PARAMS(obj, OccChrMove);
    ChrObj *o0;
    ChrObj *o;
    ChrObj *p;
    ChrObj *q;
    GphModel *m;
    s32 i;
    s32 j;
    s32 n;
    f32 r;

    switch (a->target) {
    case 0:
        o0 = ChrTenantGet(a->id);
        switch (a->mode) {
        case 0:
            if (o0->moveCmds != 0) {
                break;
            }
            if (obj->flags & OCC_CHR_TURN) {
                o0->moveDir = o0->heading = a->yaw * 182.04445f;
            }
            return;
        case 1:
            if (obj->timer > 0) {
                FldObjMoveVelocity(o0, a->vel, (obj->flags & OCC_CHR_MOVE_FLAG2) ? 2 : 0, 1.0f);
                return;
            }
            m = o0->model;
            m->pos[0] = a->pos[0];
            m->pos[1] = a->pos[1];
            m->pos[2] = a->pos[2];
            if (obj->flags & OCC_CHR_TURN) {
                o0->moveDir = o0->heading = a->yaw * 182.04445f;
            }
            return;
        }
        break;
    case 1:
    case 5:
        if (a->target == 1) {
            o = ChrGetById((u16)a->id);
        } else {
            o = ChrGetByPartySlot(a->id);
        }
        switch (a->mode) {
        case 0:
            if (o->moveCmds != 0) {
                break;
            }
            if (obj->flags & OCC_CHR_TURN) {
                o->moveDir = o->heading = a->yaw * 182.04445f;
            }
            return;
        case 1:
            if (obj->timer > 0) {
                FldObjMoveVelocity(o, a->vel, (obj->flags & OCC_CHR_MOVE_FLAG2) ? 2 : 0, 1.0f);
                return;
            }
            m = o->model;
            m->pos[0] = a->pos[0];
            m->pos[1] = a->pos[1];
            m->pos[2] = a->pos[2];
            if (obj->flags & OCC_CHR_TURN) {
                o->moveDir = o->heading = a->yaw * 182.04445f;
            }
            return;
        }
        break;
    case 2:
        r = 150.0f;
        p = ChrGetByPartySlot(1);
        n = 0;
        j = 1;
        do {
            i = j + 1;
            q = ChrGetByPartySlot(i);
            if (q != NULL && (q->flags & 1)) {
                if (FldVecDistXZ(p->model->pos, q->model->pos) < r) {
                    ChrModelAddAlpha(q->model, -3.0f);
                    if (q->flags & 8) {
                        q->flags ^= 8;
                    }
                }
                if (q->moveCmds == 0) {
                    ChrPartySlotSetActive((u8)i, 0);
                    ChrModelSetAlphaRaw(q->model, 0.0f);
                    q->flags |= 0x808;
                    if (obj->flags & OCC_CHR_TURN) {
                        q->heading = p->heading;
                    }
                } else {
                    n++;
                }
            }
            j = i;
        } while (j < 4);
        if (n == 0) {
            ChrAllStateReset();
            return;
        }
        break;
    default:
        return;
    }
    obj->timer = 1;
}
