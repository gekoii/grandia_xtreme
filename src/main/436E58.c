#include "common.h"
#include "gph.h"

f32 ChrMotionStepFrame(GphMotion *a, f32 t, f32 dt) {
    f32 r;
    u32 last;

    if (a == NULL) {
        return 0.0f;
    }
    if (a->playFlags & 1) {
        r = t + dt;
        last = a->frameCount - 1;
        while ((f32)last <= r) {
            r -= (f32)last;
        }
        return r;
    }
    last = a->frameCount - 1;
    r = t + dt;
    if ((f32)last < r) {
        r = (f32)last;
    }
    return r;
}
