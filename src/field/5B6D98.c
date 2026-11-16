/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"

void EvtAreaCalcMatrix(EvtArea *);

void OccMoveForward(OccEntry *obj) {
    OccTween *s = OCC_PARAMS(obj, OccTween);
    f32 v[3];
    GphModel *e;
    GphPart *e2;
    GphModel *e4;
    GphModel *e5;
    GphLight *l;
    EvtArea *e3;
    EvtArea *e6;
    VuVec3 *e7;
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
                        v[0] = e2->pos[0];
                        v[1] = e2->pos[1];
                        v[2] = e2->pos[2];
                        for (k = 0; k < 3; k++) {
                            if (s->delta.f[k] != 0.0f) {
                                v[k] += s->delta.f[k];
                            }
                        }
                        e2->pos[0] = v[0];
                        e2->pos[1] = v[1];
                        e2->pos[2] = v[2];
                    }
                }
            } else {
                for (i = 0; i < s->last + 1; i++) {
                    e5 = FldObjGet(s->id + i);
                    if (e5 != NULL) {
                        v[0] = e5->pos[0];
                        v[1] = e5->pos[1];
                        v[2] = e5->pos[2];
                        for (k = 0; k < 3; k++) {
                            if (s->delta.f[k] != 0.0f) {
                                v[k] += s->delta.f[k];
                            }
                        }
                        e5->pos[0] = v[0];
                        e5->pos[1] = v[1];
                        e5->pos[2] = v[2];
                    }
                }
            }
            break;
        case 1:
            for (i = 0; i <= s->last; i++) {
                e3 = EvtAreaGet(s->id + i);
                if (e3 != NULL) {
                    EvtAreaGetPos(e3, v);
                    for (k = 0; k < 3; k++) {
                        if (s->delta.f[k] != 0.0f) {
                            v[k] += s->delta.f[k];
                        }
                    }
                    EvtAreaSetPos(e3, v);
                }
            }
            break;
        case 2:
        case 4:
            break;
        case 3:
            for (i = 0; i <= s->last; i++) {
                l = FldPointLightGet(s->id + i);
                if (l != NULL) {
                    v[0] = l->dir[0];
                    v[1] = l->dir[1];
                    v[2] = l->dir[2];
                    for (k = 0; k < 3; k++) {
                        if (s->delta.f[k] != 0.0f) {
                            v[k] += s->delta.f[k];
                        }
                    }
                    l->dir[0] = v[0];
                    l->dir[1] = v[1];
                    l->dir[2] = v[2];
                }
            }
            break;
        case 5:
            for (i = 0; i <= s->last; i++) {
                e7 = FldTreasureGetModelById(s->id + i);
                if (e7 != NULL) {
                    e7->x += s->delta.f[0];
                    e7->y += s->delta.f[1];
                    e7->z += s->delta.f[2];
                    FldTreasureSetPosById(s->id + i, e7);
                }
            }
            break;
        }
        for (k = 0; k < 3; k++) {
            if (s->delta.f[k] != 0.0f) {
                s->delta.f[k] += s->accel;
            }
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
                {
                GphPart *p = ChrModelFindPart(e4, s->part);
                p->pos[0] = v[0];
                p->pos[1] = v[1];
                p->pos[2] = v[2];
                }
            } else {
                e4->pos[0] = v[0];
                e4->pos[1] = v[1];
                e4->pos[2] = v[2];
            }
            break;
        case 1:
            e6 = EvtAreaGet(s->id);
            EvtAreaSetPos(e6, v);
            if (e6->flags & 4) {
                EvtAreaCalcMatrix(e6);
            }
            break;
        case 2:
        case 4:
            break;
        case 3:
            {
                GphLight *l = FldPointLightGet(s->id);
                l->dir[0] = v[0];
                l->dir[1] = v[1];
                l->dir[2] = v[2];
            }
            break;
        case 5:
            for (i = 0; i <= s->last; i++) {
                e7 = FldTreasureGetModelById(s->id + i);
                if (e7 != NULL) {
                    e7->x = v[0];
                    e7->y = v[1];
                    e7->z = v[2];
                    FldTreasureSetPosById(s->id + i, e7);
                }
            }
            break;
        }
        break;
    }
}
