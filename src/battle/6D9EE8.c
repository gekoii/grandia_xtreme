#include "common.h"
#include "battle/n33.h"

void VuVecCopy(f32 *, s32);

void BtlHitSetNextSrcPos(s32 a) {
    f32 *o = btlHitNextSrcPos;
    VuVecCopy(o, a);
    o[3] = 1.0f;
}
