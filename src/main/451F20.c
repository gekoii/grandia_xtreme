#include "common.h"
#include "protos/main.h"
#include "main/n07.h"

void GphScreenFadeSetColor(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    GphFadeSetColor(&gphScreenFade, arg0, arg1, arg2, arg3);
}
