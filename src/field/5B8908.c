#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"

void EvtAreaCalcMatrix(EvtArea *);

void OccScaleForward(OccEntry *obj) {
    OccTween *s = OCC_PARAMS(obj, OccTween);
    f32 v[3];
    GphModel *e;
    GphPart *e2;
    GphModel *e4;
    GphModel *e5;
    EvtArea *e3;
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
        case 3:
            if (obj->flags & OCC_PART) {
                if (s->target == 3) {
                    e = FldSkyGetModel(0);
                } else {
                    e = FldObjGet(s->id);
                }
                if (e == NULL) {
                    return;
                }
                for (i = s->part; i <= s->last; i++) {
                    e2 = ChrModelFindPart(e, i);
                    if (e2 != NULL) {
                        v[0] = e2->scale[0];
                        v[1] = e2->scale[1];
                        v[2] = e2->scale[2];
                        for (k = 0; k < 3; k++) {
                            v[k] += s->delta.f[k];
                        }
                        e2->scale[0] = v[0];
                        e2->scale[1] = v[1];
                        e2->scale[2] = v[2];
                    }
                }
            } else {
                for (i = 0; i < s->last + 1; i++) {
                    if (s->target == 3) {
                        e5 = FldObjGet(s->id + i);
                    } else {
                        e5 = FldSkyGetModel(0);
                    }
                    if (e5 != NULL) {
                        v[0] = e5->scale[0];
                        v[1] = e5->scale[1];
                        v[2] = e5->scale[2];
                        for (k = 0; k < 3; k++) {
                            v[k] += s->delta.f[k];
                        }
                        e5->scale[0] = v[0];
                        e5->scale[1] = v[1];
                        e5->scale[2] = v[2];
                    }
                }
            }
            break;
        case 1:
            for (i = 0; i < s->last + 1; i++) {
                e3 = EvtAreaGet(s->id + i);
                if (e3 != NULL) {
                    e3->scale[0] += s->delta.f[0];
                    e3->scale[1] += s->delta.f[1];
                    e3->scale[2] += s->delta.f[2];
                    EvtAreaCalcMatrix(e3);
                }
            }
            break;
        }
        for (k = 0; k < 3; k++) {
            s->delta.f[k] += s->accel;
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
        case 3:
            if (s->target == 3) {
                e4 = FldSkyGetModel(0);
            } else {
                e4 = FldObjGet(s->id);
            }
            if (obj->flags & OCC_PART) {
                GphPart *e6 = ChrModelFindPart(e4, s->part);
                e6->scale[0] = v[0];
                e6->scale[1] = v[1];
                e6->scale[2] = v[2];
            } else {
                e4->scale[0] = v[0];
                e4->scale[1] = v[1];
                e4->scale[2] = v[2];
            }
            break;
        case 1: {
            EvtArea *e7 = EvtAreaGet(s->id);
            e7->scale[0] = v[0];
            e7->scale[1] = v[1];
            e7->scale[2] = v[2];
            EvtAreaCalcMatrix(e7);
            break;
        }
        }
        break;
    }
}
