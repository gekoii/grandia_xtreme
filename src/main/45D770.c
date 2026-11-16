#include "common.h"

f32 VuSinAngle(s32);

f32 VuSinf(f32 x) {
    return VuSinAngle((s32)(x * 10430.378f));
}
