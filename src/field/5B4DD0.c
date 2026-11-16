#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/event.h"
#include "field/n12.h"


void OccShakeBegin(OccEntry *obj) {
    OccShake *s = &obj->params.shake;
    GphModel *e;
    GphPart *e2;

    e = FldObjGet(s->id);
    if (e != NULL) {
        if (s->part != 0) {
            e2 = ChrModelFindPart(e, s->part);
            e2->flags &= ~2;
        }
    }
}
