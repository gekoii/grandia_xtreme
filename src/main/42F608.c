#include "common.h"
#include "gph.h"

f32 GphViewGetMagV(s32 arg0) {
    return (arg0 + gphViews)->magV;
}
