#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"

void *FldMemAlloc(s32);

s32 EffSpawnCode36(s32 arg0) {
    effMapWork->fireList[effMapWork->driftCount] = FldMemAlloc(0x12D0);
    PtclPresetDriftDust(effMapWork->fireList[effMapWork->driftCount], arg0);
    effMapWork->driftCount++;
    return (s32)effMapWork->fireList[effMapWork->driftCount - 1];
}
