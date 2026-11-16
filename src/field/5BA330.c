#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccAlphaUpdate(OccEntry *o) {
    OccTween *s = OCC_PARAMS(o, OccTween);
    f32 val;
    s32 i;
    GphModel *h;
    s32 n;
    ChrObj *e;

    switch (s->ease) {
    case 0:
        if (o->state == 4) {
            val = s->delta.f[0];
        } else {
            val = -s->delta.f[0];
        }
        switch (s->target) {
        case 0:
            if (o->flags & 0x800) {
                h = FldObjGet(s->id);
                for (i = s->part; i < s->last + 1; i++) {
                    n = ChrModelFindPart(h, i);
                    if (n != 0) {
                        ChrPartSetAlphaRaw(n, ChrPartGetAlphaRaw(n) + val);
                    }
                }
            } else {
                for (i = 0; i <= s->last; i++) {
                    h = FldObjGet(s->id + i);
                    if (h != 0) {
                        ChrModelSetAlphaRaw(h, ChrModelGetAlphaRaw(h) + val);
                    }
                }
            }
            break;
        case 1:
            for (i = 0; i < s->last + 1; i++) {
                e = ChrTenantGet(s->id + i);
                if (e != NULL) {
                    h = (s32)e->model;
                    ChrModelSetAlphaRaw(h, ChrModelGetAlphaRaw(h) + val);
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
                h = (s32)e->model;
                ChrModelSetAlphaRaw(h, ChrModelGetAlphaRaw(h) + val);
            }
            break;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        OccInterpVec(o, &val);
        switch (OCC_PARAMS(o, OccTween)->target) {
        case 0:
            if (o->flags & 0x800) {
                s32 m = ChrModelFindPart(FldObjGet(s->id), s->part);
                if (m != 0) {
                    ChrPartSetAlphaRaw(m, val);
                }
            } else {
                ChrModelSetAlphaRaw(FldObjGet(s->id), val);
            }
            break;
        case 1:
            ChrModelSetAlphaRaw((s32)ChrTenantGet(s->id)->model, val);
            break;
        case 2:
        case 4:
            if (s->target == 2) {
                e = ChrGetById(s->id);
            } else {
                e = ChrGetByPartySlot(s->id);
            }
            if (e != NULL) {
                ChrModelSetAlphaRaw((s32)e->model, val);
            }
            break;
        }
        break;
    }
}
