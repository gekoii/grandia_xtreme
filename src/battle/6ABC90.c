/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"


#define VADD_XYZ(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

void PtclBurstUpdateBasic(PtclBurst *e)
{
    f32 tmp[4] __attribute__((aligned(16)));

    if (e->active > 0) {
        if (e->unk8 != btlFrameCount) {
            PtclBurstFree(e);
        }
        return;
    }
    if (e->count < e->max) {
        e->timer += e->rate;
        while (e->timer >= 1.0f) {
            if (e->count < e->max) {
                e->active = 0;
                VADD_XYZ(tmp, ((BtlUnit *)e->unk3C)->pos, e->ofs);
                PtclBtlSprEmit(&((PtclBtlSpr *)e->parts)[e->count], tmp, e->type, 0, 1.0f);
                e->count++;
            }
            e->timer -= 1.0f;
        }
    }
    if (PtclBtlSprUpdate(e->parts, e->max, e->type) <= 0 && e->active == 0) {
        e->active = 1;
        e->unk8 = btlFrameCount;
    }
}
