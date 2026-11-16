#include "common.h"
#include "field/event.h"
#include "field/n13.h"

void OccMiscBackwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccMisc *a = OCC_PARAMS(obj, OccMisc);

    obj->timer = 1;
    a->wait = 0;
    a->unk18 = a->unk14;
}
