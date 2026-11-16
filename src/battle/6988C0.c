#include "common.h"
#include "battle/misc.h"

void FxCloneSetCircle(f32 a, f32 b) {
    FxClone *p = &fxClone;
    if (p->unit != 0) {
        p->circleMode = 1;
        p->cx = a;
        p->cz = b;
    }
}
