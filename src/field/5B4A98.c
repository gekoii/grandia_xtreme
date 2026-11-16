#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/event.h"
#include "field/n12.h"


void OccRotateBegin(OccEntry *obj) {
    OccTween *s = &obj->params.tween;
    GphModel *e;
    GphPart *part;
    GphModel *e2;
    s32 i;

    if (s->target == 0) {
        if (obj->flags & OCC_PART) {
            e = FldObjGet(s->id);
            if (e != NULL) {
                for (i = s->part; i <= s->last; i++) {
                    part = ChrModelFindPart(e, i);
                    if (part != NULL) {
                        part->flags &= ~2;
                    }
                }
            }
        } else {
            for (i = 0; i < s->last + 1; i++) {
                e2 = FldObjGet(s->id + i);
                if (e2 != NULL) {
                    e2->flags &= ~2;
                }
            }
        }
    }
}
