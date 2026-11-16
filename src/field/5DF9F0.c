#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"

void *FldMemAlloc(s32);

PtclEmitter *EffSpawnCode46(s32 arg0) {
    s32 n;

    n = effMapWork->ringCount;
    if (n >= 0x2C) {
        return 0;
    }
    effMapWork->ringList[n] = FldMemAlloc(0x12D0);
    effMapWork->ringList[n]->unk12CC = FldMemAlloc(0x40);
    PtclPresetFallingSparks(effMapWork->ringList[n], arg0);
    effMapWork->ringCount++;
    return effMapWork->ringList[n];
}
