#include "common.h"
#include "gph.h"
#include "protos/main.h"

void GphModelCalcCull(GphModel *, f32);

void GphModelModuleInit(void) {

}

void GphModelCalcAndDraw(void *arg0) {
    GphModel *model = arg0;

    if (!(model->flags & 8)) {
        GphModelCalcCull(model, model->cullScale);
        GphModelDraw(arg0);
    }
}

void GphModelDraw(GphModel *arg0) {
    GphModelDrawMtx(arg0, VuMtxStackGetTop());
}
