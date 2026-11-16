/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "gph.h"


void FxAtkWaveUpdate(FxAtk *e) {
    FxAtkWave *o = e->work;
    GphModel *s;
    GphModel *t;
    BtlUnit *p;
    BtlUnit *sl;
    s32 i;
    f32 dx, dz;

    if (o->delay > 0) {
        o->delay--;
        return;
    }
    s = o->model;
    if (s != NULL) {
        s->pos[0] += o->vx;
        s->pos[2] += o->vz;
    }
    switch (o->state) {
    case 0:
        if (s->pos[0] * s->pos[0] + s->pos[2] * s->pos[2] > 1210000.0f) {
            o->state++;
            break;
        }
        p = e->unit;
        sl = btlUnits;
        for (i = 0; i < 16; i++, sl++) {
            if (sl->state >= 0x40 && ((o->hitMask >> i) & 1)) {
                t = o->model;
                dx = sl->pos[0] - t->pos[0];
                dz = sl->pos[2] - t->pos[2];
                if (dx * dx + dz * dz <= o->radius * o->radius) {
                    FxAtkHitRegister(p, sl, BtlAtan2Angle(p->pos[0] - sl->pos[0], p->pos[2] - sl->pos[2]), 0, o->power);
                    o->hitMask &= ~(1 << i);
                }
            }
        }
        break;
    case 1:
        o->alpha -= 0.0625f;
        if (o->alpha <= 0.0f) {
            o->state++;
            e->frameStamp = btlFrameCount;
            o->model = NULL;
            return;
        }
        break;
    case 2:
        if (e->frameStamp != btlFrameCount) {
            FxAtkFree(e);
        }
        return;
    }
    ChrModelUpdate(o->model, 1.0f);
}
