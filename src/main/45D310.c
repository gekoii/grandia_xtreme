#include "common.h"
#include "protos/main.h"
s16 fptoui(f32);
s32 VuAcosAngle(f32 x) {
    return fptoui(VuAcosf(x) * 10430.378f);
}
