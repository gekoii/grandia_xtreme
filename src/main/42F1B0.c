#include "common.h"
#include "protos/main.h"


f32 VuRandSigned(f32 f) {
    s32 a = rand();
    return ((f32)a - (f32)rand()) * 2.3283064e-10f * f;
}
