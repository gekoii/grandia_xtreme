#include "common.h"
#include "field/event.h"
#include "field/n13.h"

void OccPartCollapseBackwardStart(void *arg0) {
    OccEntry *obj = arg0;

    obj->timer = OCC_PARAMS(obj, OccPartCollapse)->backTime + 1;
}
