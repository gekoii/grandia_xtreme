#include "common.h"

f32 VuSinAngle(s32);

f32 VuCosAngle(s32 arg0) {
    return VuSinAngle(arg0 + 0x4000);
}
