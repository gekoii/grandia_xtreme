/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"

#include "battle/misc.h"
#include "battle/n30.h"

void VuVecCopy(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);

void FxTrailUpdate(void) {
    f32 v[4] __attribute__((aligned(16)));
    FxTrail *e = fxTrails;
    s32 i;
    FxTraceOfs *p;
    FxTraceSpark *t;
    s32 k;

    for (i = 0; i < 32; i++, e++) {
        if (e->handle == NULL) {
            continue;
        }
        VuVecCopy(v, e->handle[e->unk18].a);
        if (++e->unk18 >= e->unk1A) {
            e->unk18 = 0;
        }
        p = &e->handle[e->unk18];
        VuVecCopy(p->a, e->unk8);
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf2, 0(%2)\n\t"
            "vsub.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(v), "r"(p->a), "r"(v) : "memory");
        VuVecNormalize(p->b, v);
        if (btlActingUnit == 0xFF || (e->owner->flags & 8)) {
            e->unkC += 0.0625f;
            if (e->unkC > 1.0f) {
                e->unkC = 1.0f;
            }
            k = e->unk21;
            if (k >= 0) {
                t = &fxTraceSparks[k];
                if (t->state <= 0) {
                    PtclBtlSprEmit(&t->handle[t->frame], p, t->unk3, 1, e->unk24);
                    if (++t->frame >= t->count) {
                        t->frame = 0;
                    }
                }
            }
        } else {
            e->unkC -= 0.0625f;
            if (e->unkC < 0.0f) {
                e->unkC = 0.0f;
            }
        }
    }
}
