/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/misc.h"

void FxCloneSetAlpha(s8 i, s16 dur, f32 v) {
    FxClone *p = &fxClone;
    FxCloneEnt *e;

    if (p->unit != 0 && i < p->count) {
        e = &p->clones[i];
        if (dur <= 0) {
            e->step = 0.0f;
            e->target = v;
            e->alpha = v;
            return;
        }
        e->target = v;
        e->step = (v - e->alpha) / (f32)dur;
    }
}
