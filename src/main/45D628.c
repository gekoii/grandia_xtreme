/* cflags: -fno-delayed-branch */
/* without -fno-delayed-branch the div.s moves into the jal delay slot (the original leaves it before the prologue) */
#include "common.h"
#include "protos/main.h"


f32 VuAtanYX(f32 y, f32 x) {
    return VuAtanf(y / x);
}
