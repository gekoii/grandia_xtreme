#include "common.h"
#include "field/event.h"
#include "field/n13.h"

void OccAlphaPulseBackwardStart(void *arg0) {
    OccEntry *obj = arg0;

    obj->timer = OCC_PARAMS(obj, OccAlphaPulse)->backTime;
}

void OccAlphaPulseBackward(void) {

}
