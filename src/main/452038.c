#include "common.h"
#include "protos/main.h"
#include "main/n07.h"

void GphScreenFadeStart(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    GphFadeStart(&gphScreenFade, arg0, arg1, arg2, arg3, arg4, arg5);
}

s32 GphFadeIsActive(GphFade *arg0) {
    return arg0->active;
}
