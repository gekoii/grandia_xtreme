#include "common.h"
#include "battle/n30.h"

void IpGaugeSetActor(s32 arg0) {
    ipGauge.highlight = 1.0f;
    ipGauge.actor = arg0;
    ipGauge.targetMask = 0;
    ipGauge.areaMask = 0;
}
