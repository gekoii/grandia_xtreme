#include "common.h"
#include "battle/misc.h"

s32 FxCloneIsCircleOf(s32 arg0) {
    if (((s32)fxClone.unit != arg0) || (fxClone.circleMode != 1)) {
        return 0;
    }
    return 1;
}
