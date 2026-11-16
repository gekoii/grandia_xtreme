#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccObjParamForwardStart(OccEntry *o) {
    OccObjParam *s = OCC_PARAMS(o, OccObjParam);
    GphModel *e;

    switch (s->type) {
    case 0:
        switch (s->op) {
        case 1:
            e = FldObjGet(OCC_PARAMS(o, OccObjParam)->id);
            if (e == NULL) {
                OccEnd(o);
                return;
            }
            s->saved = e->cullSphere[3];
            e->cullSphere[3] = s->value;
            o->timer = s->time;
            break;
        case 0:
        case 2:
        case 3:
            o->timer = 1;
            break;
        }
        break;
    case 1:
    case 2:
        o->timer = 1;
        break;
    }
}
