#include "common.h"
#include "battle/n30.h"

void IpTransStart(s32 arg0, f32 arg1) {
    if (arg0 <= 0) {
        ipTransDelay = 0;
        ipTransDelta = arg1;
        return;
    }
    ipTransDelay = arg0;
    ipTransNextDelta = arg1;
}
