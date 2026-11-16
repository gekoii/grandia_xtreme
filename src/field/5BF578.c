#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "pad.h"
#include "protos/field.h"

void OccVibrateForwardStart(u8 *arg0) {
    OccVibrate *a = OCC_PARAMS(arg0, OccVibrate);

    PadVibrate(padPort0, a->motor, a->level, a->time);
}
