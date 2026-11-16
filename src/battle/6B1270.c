#include "common.h"
#include "battle/misc.h"

void MgcSetMotionTime(f32 fparg0) {
    mgcMotionTime = fparg0;
}

f32 MgcGetMotionTime(void) {
    return mgcMotionTime;
}
