#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccUvScrollForward(u8 *arg0) {
    OccUvScroll *a = OCC_PARAMS(arg0, OccUvScroll);
    f32 buf[4];
    GphModel *m;
    s32 i;
    GphPart *o;

    m = FldObjGet(a->id);
    if (m == 0) {
        return;
    }
    if (a->ease != 0) {
        if (a->ease >= 0) {
            if (a->ease < 5) {
                OccInterpVec(arg0, buf);
            }
        }
    } else {
        if (a->part == 0 && a->last == 0) {
            o = ChrModelPartAt(m, 0);
            i = 0;
            while (o != NULL) {
                if (a->u != 0 || a->v != 0) {
                    ChrPartSetAttr(o, 3, 1);
                    o->uvScroll[1] = a->u;
                    o->uvScroll[3] = a->v;
                } else {
                    ChrPartSetAttr(o, 3, 0);
                    o->uvScroll[1] = 0;
                    o->uvScroll[3] = 0;
                }
                i++;
                o = ChrModelPartAt(m, i);
            }
        } else {
            for (i = a->part; i <= a->last; i++) {
                o = ChrModelFindPart(m, i);
                if (o != NULL) {
                    if (a->u != 0 || a->v != 0) {
                        ChrPartSetAttr(o, 3, 1);
                        o->uvScroll[1] = a->u;
                        o->uvScroll[3] = a->v;
                    } else {
                        ChrPartSetAttr(o, 3, 0);
                        o->uvScroll[1] = 0;
                        o->uvScroll[3] = 0;
                    }
                }
            }
        }
    }
}
