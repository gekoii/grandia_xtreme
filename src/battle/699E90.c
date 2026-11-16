#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "gph.h"


void FxAtkWaveSpawnEcho(BtlUnit *arg0, FxAtkWave *src, s32 arg2) {
    FxAtk *e;
    FxAtkWave *o;
    GphModel *s;

    e = FxAtkAlloc(arg0, 1);
    if (e == NULL) {
        return;
    }
    o = MemAlloc16A(0x20);
    e->work = o;
    if (o == NULL) {
        FxAtkFree(e);
        return;
    }
    s = BtlModelCreateWithPool(fxAtkWaveModel, &e->pool);
    o->model = s;
    if (s == NULL) {
        FxAtkFree(e);
        return;
    }
    o->vx = src->vx;
    s->rot[1] = src->model->rot[1];
    o->vz = src->vz;
    o->hitMask = src->hitMask;
    s->pos[0] = src->model->pos[0];
    s->pos[2] = src->model->pos[2];
    o->radius = src->radius;
    o->alpha = 1.0f;
    o->delay = arg2;
    o->state = 0;
    e->update = FxAtkWaveUpdate;
    o->power = src->power;
    e->build = FxAtkWaveBuild;
}
