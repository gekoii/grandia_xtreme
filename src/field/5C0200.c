#include "common.h"
#include "protos/field.h"


f32 OccRandSpread(f32 arg0) {
    return ((f32)VuRandInt((s32)arg0) - 0.5f) * (arg0 + arg0);
}
