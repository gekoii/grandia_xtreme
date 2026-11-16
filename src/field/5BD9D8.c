#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccMiscForward(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccMisc *a = OCC_PARAMS(obj, OccMisc);
    s16 r;

    if (a->wait > 0) {
        a->wait--;
        obj->timer = 1;
    } else {
        obj->timer = 0;
    }
    if (a->op == 6) {
        r = CamBlendIsActive();
        if (r == 1) {
            obj->timer = r;
        }
    }
}
