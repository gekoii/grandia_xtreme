#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "gph.h"


void FxAtkRingBuild(FxAtk *e) {
    FxAtkRing *o = e->work;
    f32 f;

    if (o->model != NULL && o->delay <= 0) {
        ChrModelSetAlpha(o->model, (e->alpha * o->alpha - 1.0f) * 128.0f);
        f = o->t * (o->radius * 0.00043333330540917814f);
        o->model->scale[2] = f;
        o->model->scale[0] = f;
        BtlChrModelCalcDraw(o->model);
    }
}
