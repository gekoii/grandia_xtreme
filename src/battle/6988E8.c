#include "common.h"
#include "battle/misc.h"

s32 FxCloneGetCircle(f32 *out) {
    FxClone *p = &fxClone;

    if (p->unit == NULL || p->circleMode != 1) {
        return 0;
    }
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(out) : "memory");
    out[0] = p->cx;
    out[2] = p->cz;
    return p->count;
}
