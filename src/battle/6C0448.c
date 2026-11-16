#include "common.h"
#include "battle/unit.h"

void BtlUnitLungeSet(s32 *arg0, f32 a, f32 b) {
    btlUnitLunge[*arg0].speed = a;
    btlUnitLunge[*arg0].decel = b;
}
