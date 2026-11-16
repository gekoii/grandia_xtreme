#include "common.h"

void GphModelCalcCull(void *, f32);

void GphModelCalc(void *arg0) {
    GphModelCalcCull(arg0, 1.0f);
}
