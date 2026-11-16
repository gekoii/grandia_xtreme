#include "common.h"
#include "protos/main.h"
f32 SndEqFilterEnergy(void *, s32 *);
s32 SndFloatToInt(f32);
s32 SndCalcBandLevel(void *arg0, s32 *arg1) {
    return SndFloatToInt(log10f(SndEqFilterEnergy(arg0, arg1) + 1.0f) * 20.0f);
}
