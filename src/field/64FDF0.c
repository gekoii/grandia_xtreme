#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void MiniBallEnd(void) {
    GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f);
    GphScreenFxSet(fldScreenFx, 2, 0, 10, 0.0f);
}
