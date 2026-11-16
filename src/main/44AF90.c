#include "common.h"
#include "ptcl.h"

void PtclCtrlTracksClear(u8 *arg0) {
    PtclCtrl *c = (PtclCtrl *)arg0;

    c->length = 0;
    c->genFlags = 0;
    c->effectorStep.on = 0;
    c->posX.lastKey = 0;
    c->posY.lastKey = 0;
    c->posZ.lastKey = 0;
    c->rotX.lastKey = 0;
    c->rotY.lastKey = 0;
    c->rotZ.lastKey = 0;
    c->color.lastKey = 0;
    c->emitAngX.lastKey = 0;
    c->emitAngY.lastKey = 0;
    c->speed.lastKey = 0;
    c->accel.lastKey = 0;
    c->life.lastKey = 0;
    c->rate.lastKey = 0;
    c->angX.lastKey = 0;
    c->angY.lastKey = 0;
    c->radius.lastKey = 0;
    c->onceNum.lastKey = 0;
    c->sinX.lastKey = 0;
    c->sxCurveStep.on = 0;
    c->sxTime.lastKey = 0;
    c->sx0.lastKey = 0;
    c->sxRate.lastKey = 0;
    c->sinY.lastKey = 0;
    c->syCurveStep.on = 0;
    c->syTime.lastKey = 0;
    c->sy0.lastKey = 0;
    c->syRate.lastKey = 0;
    c->colorCurveStep.on = 0;
    c->colorTime.lastKey = 0;
    c->colorKeys.lastKey = 0;
    c->rotXAnim.lastKey = 0;
    c->rdxModeStep.on = 0;
    c->rotYAnim.lastKey = 0;
    c->rdyModeStep.on = 0;
    c->rotZAnim.lastKey = 0;
    c->rdzModeStep.on = 0;
}
