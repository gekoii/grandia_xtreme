#include "common.h"
#include "gph.h"

void GphViewSetClip(s32 arg0, f32 a, f32 b) {
    GphView *p = &gphViews[arg0];
    p->clipNear = a;
    p->clipFar = b;
}
