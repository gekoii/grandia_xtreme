/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "gph.h"


void FxAtkRingUpdate(FxAtk *e) {
    FxAtkRing *o = e->work;
    BtlUnit *sl;
    GphModel *s;
    BtlUnit *p;
    s32 i;
    f32 dx, dz, r;

    if (o->delay > 0) {
        o->delay--;
        return;
    }
    switch (o->state) {
    case 0:
        o->t += 0.05f;
        if (1.0f <= o->t) {
            o->state++;
            e->frameStamp = btlFrameCount;
            o->model = NULL;
            return;
        }
        if (o->t > 0.75f) {
            o->alpha = (1.0f - o->t) * 4.0f;
        }
        p = e->unit;
        sl = btlUnits;
        for (i = 0; i < 16; i++, sl++) {
            if (sl->state >= 0x40 && ((o->hitMask >> i) & 1)) {
                s = o->model;
                dx = sl->pos[0] - s->pos[0];
                dz = sl->pos[2] - s->pos[2];
                r = o->radius * o->t;
                if (dx * dx + dz * dz <= r * r) {
                    FxAtkHitRegister(p, sl, BtlAtan2Angle(s->pos[0] - sl->pos[0], s->pos[2] - sl->pos[2]), 0, o->power);
                    o->hitMask &= ~(1 << i);
                }
            }
        }
        break;
    case 1:
        if (e->frameStamp != btlFrameCount) {
            FxAtkFree(e);
        }
        return;
    }
    ChrModelUpdate(o->model, 1.0f);
}
