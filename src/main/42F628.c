#include "common.h"
#include "protos/main.h"


void GphViewSetup(s32 arg0, s32 x, s32 y, s32 w, s32 h, f32 fparg0, f32 fparg1, f32 fparg2) {
    if (arg0 < 8) {
        GphViewSetArea(arg0, x, y, w, h);
        GphViewSetClip(arg0, fparg1, fparg0);
        GphViewSetFov(arg0, fparg2);
    }
}
