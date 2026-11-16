#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxAtkWaveBuild(FxAtk *e) {
    FxAtkWave *o = e->work;

    if (o->model != NULL && o->delay <= 0) {
        ChrModelSetAlpha(o->model, (e->alpha * o->alpha - 1.0f) * 128.0f);
        BtlChrModelCalcDraw(o->model);
    }
}
