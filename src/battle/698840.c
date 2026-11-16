/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/misc.h"

void FxCloneFadeOut(s16 dur) {
    FxClone *p = &fxClone;
    FxCloneEnt *e;
    s32 i;

    if (p->unit != 0) {
        if (dur <= 0) {
            dur = 1;
        }
        p->unk4C = dur;
        e = p->clones;
        for (i = 0; i < p->count; i++) {
            e->target = 0.0f;
            e->step = (e->target - e->alpha) / (f32)dur;
            e++;
        }
    }
}
