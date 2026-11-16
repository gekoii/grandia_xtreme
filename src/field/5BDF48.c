#include "common.h"
#include "field/event.h"
#include "field/n13.h"

void OccLightBackwardStart(void *arg0) {
    OccEntry *obj = arg0;

    obj->timer = OCC_PARAMS(obj, OccTween)->time;
}

void OccUvScrollForwardStart(s16 *arg0) {
    ((OccEntry *)arg0)->timer = 1;
}
