#include "common.h"
#include "battle/n30.h"

void IpIconSetPos(s32 arg0, f32 arg1) {
    ipGauge.icons[arg0].pos = 50.0f - arg1 * 50.0f;
}
