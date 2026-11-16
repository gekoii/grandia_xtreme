#include "common.h"
#include "protos/battle.h"

u16 BtlAtan2Angle(f32 y, f32 x) {
    return atan2f(y, x) * 10430.381f;
}
