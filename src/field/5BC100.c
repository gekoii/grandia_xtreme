#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccCamScriptForward(OccEntry *o) {
    OccCamScript *s = OCC_PARAMS(o, OccCamScript);
    if (s->mode == 0) {
        CamScrStart(s->script);
    }
}
