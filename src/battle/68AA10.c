#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n28.h"

f32 VuSinAngle(s32);

void PtclBtlDustRingUpdate(s32 idx) {
    PtclBtlDustRing *e;
    PtclBtlDust *s;
    f32 r;
    s32 i;
    s32 j;

    e = ptclBtlDustGroups[idx];
    r = VuSinAngle((f32)e->timer * 682.6667f) + e->spread;
    for (i = 0; i < 32; i++) {
        s = e->sprites[i];
        if (s != NULL) {
            s->unk10 = e->x + e->vx[i] * r;
            s->unk18 = e->z + e->vz[i] * r;
            s->unk0 -= 0.041666668f;
        }
    }
    if (++e->timer >= 24) {
        for (j = 0; j < 32; j++) {
            if (e->sprites[j] != NULL) {
                e->sprites[j]->unk0 = 0.0f;
            }
        }
        MemFree(ptclBtlDustGroups[idx]);
        ptclBtlDustGroups[idx] = NULL;
    }
}
