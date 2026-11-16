#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"

void *FldMemAlloc(s32);

s32 EffSpawnCode01(s32 arg0) {
    s32 n;

    n = effMapWork->fireCount;
    if (n >= 0x30) {
        return 0;
    }
    effMapWork->fireList[n] = FldMemAlloc(0x12D0);
    PtclPresetFire(effMapWork->fireList[n], arg0);
    (effMapWork->fireCount)++;
    return (s32)effMapWork->fireList[n];
}
