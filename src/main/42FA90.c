#include "common.h"
#include "gph.h"

extern s32 VuFloatToInt(f32);

void GphViewGetArea(s32 arg0, s32 *arg1) {
    GphView *e = &gphViews[arg0];
    arg1[0] = VuFloatToInt(e->x);
    arg1[1] = VuFloatToInt(e->y);
    arg1[2] = VuFloatToInt(e->w);
    arg1[3] = VuFloatToInt(e->h);
}
