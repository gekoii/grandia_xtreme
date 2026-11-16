#include "common.h"
#include "protos/battle.h"

s32 BtlAsinAngle(f32 x) {
    return (u16)(asinf(x) * 10430.381f);
}
