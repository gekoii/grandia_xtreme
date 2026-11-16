#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"


void OccLensFlareForward(u8 *arg0) {
    OccLensFlare *a = OCC_PARAMS(arg0, OccLensFlare);

    FldLensFlareSet(a->x, a->y, a->z, a->enable);
}
