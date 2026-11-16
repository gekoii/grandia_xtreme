#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"

void *FldMemAlloc(s32);

s32 EffSpawnCode45(s32 arg0) {
    s32 n;

    n = effMapWork->moteCount;
    if (n >= 0x30) {
        return 0;
    }
    effMapWork->moteList[n] = FldMemAlloc(0x12D0);
    PtclPresetFloatingMotes(effMapWork->moteList[n], arg0);
    effMapWork->moteCount++;
    return (s32)effMapWork->moteList[n];
}
