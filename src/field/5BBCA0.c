#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"


void OccTrapForwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;

    if (ChrGetByNo(1)->blinkTimer != 0) {
        OccEnd(obj);
        return;
    }
    obj->timer = 1;
    CamSetEyeMode(1);
}
