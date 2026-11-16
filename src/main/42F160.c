#include "common.h"
#include "protos/main.h"

s32 VuFloatToInt(f32);

s32 VuRandInt(s32 arg0) {
    return VuFloatToInt((f32)(rand() >> 16) * (1.0f / 32768.0f) * (f32)arg0);
}
