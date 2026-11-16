#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"


void PtclBurstDrawBasic(PtclBurst *arg0) {
    PtclBtlSprDraw(arg0->parts, arg0->max, arg0->type, 1.0f);
}
