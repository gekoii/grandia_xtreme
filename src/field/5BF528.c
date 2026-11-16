#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccMapReloadForwardStart(u8 *arg0) {
    EvtSaveMapCheckpoint(OCC_PARAMS(arg0, OccMapReload)->unk0, OCC_PARAMS(arg0, OccMapReload)->unk1);
    GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 128.0f, 20.0f);
    MapReenterRequest(0);
}
