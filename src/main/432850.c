#include "common.h"
#include "vu.h"

f32 VuVecNormalizeLen(FVECTOR *, FVECTOR *);

void VuMtxGetScale(f32 *arg0, s32 arg1, FVECTOR *arg2) {
    FVECTOR tmp;

    arg0[0] = VuVecNormalizeLen(&tmp, &arg2[0]);
    arg0[1] = VuVecNormalizeLen(&tmp, &arg2[1]);
    arg0[2] = VuVecNormalizeLen(&tmp, &arg2[2]);
}
