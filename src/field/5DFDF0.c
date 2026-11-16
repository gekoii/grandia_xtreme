#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"

void *FldMemAlloc(s32);

s32 EffSpawnUnusedD(s32 arg0) {
    s32 n;

    n = effMapWork->charaCount;
    if (n >= 0x30) {
        return 0;
    }
    effMapWork->charaPool[n] = FldMemAlloc(0x12D0);
    PtclPresetAppear(effMapWork->charaPool[n], arg0);
    effMapWork->charaCount++;
    return (s32)effMapWork->charaPool[n];
}
