#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"


void EffSpawnSpotOpen(s32 arg0) {
    PtclPresetTreasureOpenPuff(effMapWork->spotOpenPool[0], arg0);
}
