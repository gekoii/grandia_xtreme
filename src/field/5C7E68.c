#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n14.h"

void ChrMotStateReset(GphModel *a) {
    if (a != NULL) {
        a->motionMode = 0;
        a->blend = 0.0f;
        a->motion[0].frame = 0.0f;
        a->motion[1].frame = 0.0f;
        a->motion[0].motion = NULL;
        a->motion[1].motion = NULL;
    }
}
