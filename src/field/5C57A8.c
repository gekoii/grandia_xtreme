#include "common.h"
#include "field/state.h"

void FldHpGaugeInit(FldHpGauge *arg0, s32 arg1, s32 arg2) {
arg0->max = arg2;
arg0->flag = 0xFF;
arg0->animT = 0;
arg0->cur = arg1;
arg0->shown = arg1;
}
