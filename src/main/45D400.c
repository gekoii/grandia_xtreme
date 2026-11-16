/* cflags: -fno-delayed-branch */
/* without -fno-delayed-branch the div.s moves into the jal delay slot (the original leaves it before the prologue) */
#include "common.h"
#include "protos/main.h"

s32 fptoui(f32);

s32 VuAtanYXAngle(f32 y, f32 x) {
    return fptoui(VuAtanf(y / x) * 10430.378f);
}
