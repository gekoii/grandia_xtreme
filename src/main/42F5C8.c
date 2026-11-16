#include "common.h"
#include "gph.h"

void GphViewSetMagV(s32 arg0, f32 arg1) {
    GphView *e = &gphViews[arg0];
    e->magV = arg1;
}
