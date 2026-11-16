#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


void FxCastAuraRegister(BtlUnit *arg0) {
    FxCastAura *p = &fxCastAura[arg0->id];
    VuVec3 v;

    if (p->model != NULL) {
        BtlChrGetPartPos(&v, arg0, 1);
        p->model->pos[0] = v.x;
        p->model->pos[1] = v.y;
        p->model->pos[2] = v.z;
        ChrModelSetAlpha(p->model, (p->alpha * arg0->alpha - 1.0f) * 128.0f);
        BtlChrModelCalcDraw(p->model);
    }
}
