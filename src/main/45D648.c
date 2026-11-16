#include "common.h"

s32 VuSinAngle(s32);

s32 VuCosf(f32 a) {
    return VuSinAngle((a + 1.570796f) * 10430.378f);
}
