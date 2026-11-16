#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccObjParamForward(OccEntry *o) {
    OccObjParam *s = OCC_PARAMS(o, OccObjParam);
    s32 i;

    switch (s->type) {
    case 0:
        switch (s->op) {
        case 0:
            for (i = 0; i <= s->last; i++) {
                GphModel *e = FldObjGet(s->id + i);
                if (e != NULL) {
                    e->cullSphere[3] = s->value;
                }
            }
            break;
        case 1: {
            GphModel *e;
            e = FldObjGet(OCC_PARAMS(o, OccObjParam)->id);
            if (o->timer == 0) {
                e->cullSphere[3] = s->saved;
            }
            break;
        }
        case 2: {
            GphModel *e;
            e = FldObjGet(OCC_PARAMS(o, OccObjParam)->id);
            occObjParamSlots[s->slot] = e->cullSphere[3];
            e->cullSphere[3] = s->value;
            break;
        }
        case 3: {
            GphModel *e;
            e = FldObjGet(OCC_PARAMS(o, OccObjParam)->id);
            e->cullSphere[3] = occObjParamSlots[s->slot];
            break;
        }
        }
        break;
    case 1:
        if (s->part == 0) {
            for (i = 0; i <= s->last; i++) {
                FldObjGet(s->id + i);
            }
        } else {
            GphModel *e;
            e = FldObjGet(OCC_PARAMS(o, OccObjParam)->id);
            if (e != NULL) {
                for (i = 0; i <= s->last; i++) {
                    ChrModelFindPart(e, s->part + i);
                }
            }
        }
        break;
    case 2:
        for (i = 0; i <= s->last; i++) {
            FldObjGet(s->id + i);
        }
        break;
    }
}
