#include "common.h"
#include "gph.h"

f32 GphViewGetMagH(s32 arg0) {
    GphView *p = &gphViews[arg0];
    return p->magH;
}
