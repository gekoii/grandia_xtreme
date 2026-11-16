#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


f32 BtlChrGetCenterY(s32 arg0) {
    VuVec3 v;

    BtlChrGetPartPos(&v, arg0, 1);
    return v.y;
}
