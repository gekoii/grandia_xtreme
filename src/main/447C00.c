#include "common.h"
#include "protos/main.h"

void GphFbRippleDraw(void);

void GphFbRipple(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    GphFbRippleBuild(arg0, arg1, arg2, arg3, arg4);
    GphFbRippleDraw();
}
