#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"

void EvtAreaCalcMatrix(EvtArea *);

void OccRotateForward(OccEntry *obj) {
    OccTween *s = OCC_PARAMS(obj, OccTween);
    f32 v[3];
    GphModel *e;
    GphPart *e2;
    GphModel *e4;
    GphModel *e5;
    EvtArea *e3;
    EvtArea *e6;
    ChrObj *m;
    ChrObj *m2;
    GphModel *p;
    GphModel *p4;
    ChrObj *m4;
    GphPart *e8;
    GphModel *p3;
    ChrObj *m3;
    s32 i;
    s32 k;

    switch (s->ease) {
    case 0:
        if (s->count > 0) {
            s->count--;
            obj->timer++;
            break;
        }
        switch (OCC_PARAMS(obj, OccTween)->target) {
        case 0:
            if (obj->flags & OCC_PART) {
                e = FldObjGet(s->id);
                if (e == NULL) {
                    return;
                }
                for (i = s->part; i <= s->last; i++) {
                    e2 = ChrModelFindPart(e, i);
                    if (e2 != NULL) {
                        ((s32 *)v)[0] = e2->rot[0];
                        ((s32 *)v)[1] = e2->rot[1];
                        ((s32 *)v)[2] = e2->rot[2];
                        for (k = 0; k < 3; k++) {
                            if (s->delta.i[k] != 0) {
                                ((s32 *)v)[k] += s->delta.i[k];
                            }
                        }
                        e2->rot[0] = ((s32 *)v)[0];
                        e2->rot[1] = ((s32 *)v)[1];
                        e2->rot[2] = ((s32 *)v)[2];
                    }
                }
            } else {
                for (i = 0; i < s->last + 1; i++) {
                    e5 = FldObjGet(s->id + i);
                    if (e5 != NULL) {
                        ((s32 *)v)[0] = e5->rot[0];
                        ((s32 *)v)[1] = e5->rot[1];
                        ((s32 *)v)[2] = e5->rot[2];
                        for (k = 0; k < 3; k++) {
                            if (s->delta.i[k] != 0) {
                                ((s32 *)v)[k] += s->delta.i[k];
                            }
                        }
                        e5->rot[0] = ((s32 *)v)[0];
                        e5->rot[1] = ((s32 *)v)[1];
                        e5->rot[2] = ((s32 *)v)[2];
                    }
                }
            }
            break;
        case 1:
            for (i = 0; i < s->last + 1; i++) {
                e3 = EvtAreaGet(s->id + i);
                if (e3 != NULL) {
                    for (k = 0; k < 3; k++) {
                        if (s->delta.i[k] != 0) {
                            e3->rot[k] += s->delta.i[k];
                        }
                    }
                    EvtAreaCalcMatrix(e3);
                }
            }
            break;
        case 2:
            for (i = 0; i < s->last + 1; i++) {
                m3 = ChrTenantGet(s->id + i);
                p3 = m3->model;
                p3->rot[0] += s->delta.i[0];
                m3->moveDir = m3->heading = m3->heading + s->delta.i[1];
                p3->rot[2] += s->delta.i[2];
            }
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
            p = m->model;
            p->rot[0] += s->delta.i[0];
            m->moveDir = m->heading = m->heading + s->delta.i[1];
            p->rot[2] += s->delta.i[2];
            break;
        }
        s->count = s->interval;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        OccInterpVec(obj, v);
        switch (OCC_PARAMS(obj, OccTween)->target) {
        case 0:
            e4 = FldObjGet(s->id);
            if (obj->flags & OCC_PART) {
                f32 x, y, z;
                e8 = ChrModelFindPart(e4, s->part);
                x = v[0] * 182.04445f;
                y = v[1] * 182.04445f;
                z = v[2] * 182.04445f;
                e8->rot[0] = x;
                e8->rot[1] = y;
                e8->rot[2] = z;
            } else {
                f32 x, y, z;
                x = v[0] * 182.04445f;
                y = v[1] * 182.04445f;
                z = v[2] * 182.04445f;
                e4->rot[0] = x;
                e4->rot[1] = y;
                e4->rot[2] = z;
            }
            break;
        case 1:
            e6 = EvtAreaGet(s->id);
            for (k = 0; k < 3; k++) {
                e6->rot[k] = v[k] * 182.04445f;
            }
            EvtAreaCalcMatrix(e6);
            break;
        case 2: {
            f32 x, y, z;
            m4 = ChrTenantGet(s->id);
            x = v[0] * 182.04445f;
            y = v[1] * 182.04445f;
            z = v[2] * 182.04445f;
            p4 = m4->model;
            p4->rot[0] = x;
            m4->moveDir = m4->heading = y;
            p4->rot[2] = z;
            break;
        }
        case 3:
        case 4: {
            f32 x, y, z;
            if (s->target == 3) {
                m2 = ChrGetById(s->id);
            } else {
                m2 = ChrGetByPartySlot(s->id);
            }
            x = v[0] * 182.04445f;
            y = v[1] * 182.04445f;
            z = v[2] * 182.04445f;
            m2->model->rot[0] = x;
            m2->moveDir = m2->heading = y;
            m2->model->rot[2] = z;
            break;
        }
        }
        break;
    }
}
