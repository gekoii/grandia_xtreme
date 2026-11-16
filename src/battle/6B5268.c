#include "common.h"
#include "battle/env.h"
#include "protos/battle.h"

void EnvLightSelectForUnit(s32 *p) {
    EnvLightSelectDim(((envDimMask >> *p) ^ 1) & 1);
}
