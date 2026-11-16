#include "common.h"
#include "battle/n30.h"
#include "m2c_macros.h"

void IpGaugeSetShown(s32 arg0) {
    if (arg0 == 0) {
        ipGauge.shown = 0;
        return;
    }
    ipGauge.shown = 1;
}
