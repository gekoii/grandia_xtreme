#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/event.h"
#include "field/n12.h"


void OccPartPairBegin(OccEntry *obj) {
    OccPartPair *s = &obj->params.partPair;
    GphPart *list[2];
    GphModel *e;
    GphPart *e2;
    s32 i;

    e = FldObjGet(s->id);
    if (e != NULL) {
        for (i = 0; i < 2; i++) {
            if (s->part[i] != 0) {
                e2 = list[i] = ChrModelFindPart(e, s->part[i]);
                if (e2 != NULL) {
                    switch (s->mode) {
                    case 0:
                        e2->flags &= ~2;
                        break;
                    case 1:
                        e2->flags &= ~1;
                        break;
                    case 2:
                        e2->flags &= ~4;
                        break;
                    }
                }
            }
        }
    }
}
