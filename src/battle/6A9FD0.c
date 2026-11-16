#include "common.h"
#include "protos/battle.h"

void BtlHudUpdate(void) {
    IpPanelUpdate();
    IpGaugeUpdate();
    BtlHudDmgUpdate();
}
