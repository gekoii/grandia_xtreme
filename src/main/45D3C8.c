#include "common.h"
#include "protos/main.h"
s32 fptoui(f32);
s32 VuAtanAngle(f32 x) {
    return fptoui(VuAtanf(x) * 10430.378f);
}
