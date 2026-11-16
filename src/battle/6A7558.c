#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


void IpGaugeSetTargets(void) {
    s32 t;

    t = btlUnits[ipGauge.actor].target;

    if (t >= 4 && btlUnits[t].action != 0x7F) {
        ipGauge.areaMask = ((s32 (*)(BtlUnit *))BtlUnitCollectAreaTargetsAtTarget)(&btlUnits[t]);
    }
    ipGauge.targetMask = ((s32 (*)(BtlUnit *))BtlUnitCollectAreaTargetsAtTarget)(&btlUnits[ipGauge.actor]);
    ipGauge.areaMask |= 0x10000;
}
