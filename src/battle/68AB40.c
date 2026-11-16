#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n28.h"

f32 VuSinAngle(s32);

void PtclBtlDustSprayUpdate(s32 idx) {
    PtclBtlDustSpray *e;
    PtclBtlDust *s;
    f32 r;
    s32 i;
    s32 j;

    e = ptclBtlDustGroups[idx];
    for (i = 0; i < 4; i++) {
        s = e->sprites[i];
        if (s != NULL) {
            r = VuSinAngle((f32)e->timer * 682.6667f);
            s->unk10 = e->x + e->vx[i / 2] * r;
            s->unk18 = e->z + e->vz[i / 2] * r;
            if (i & 1) {
                s->unk10 += e->vx[i / 2] * 0.25f;
                s->unk18 += e->vz[i / 2] * 0.25f;
            }
            s->unk0 -= 0.041666668f;
        }
    }
    if (++e->timer >= 24) {
        for (j = 0; j < 4; j++) {
            if (e->sprites[j] != NULL) {
                e->sprites[j]->unk0 = 0.0f;
            }
        }
        MemFree(ptclBtlDustGroups[idx]);
        ptclBtlDustGroups[idx] = NULL;
    }
}
