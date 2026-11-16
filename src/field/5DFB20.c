#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"

void *FldMemAlloc(s32);

s32 EffSpawnCode11(s32 arg0) {
    s32 n;

    n = effMapWork->steamCount;
    if (n >= 0x20) {
        return 0;
    }
    effMapWork->steamList[n] = FldMemAlloc(0x12D0);
    PtclPresetSteam(effMapWork->steamList[n], arg0);
    effMapWork->steamCount++;
    return (s32)effMapWork->steamList[n];
}
