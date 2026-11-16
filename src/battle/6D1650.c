#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"


void BtlHudDmgUpdate(void) {
    BtlHudDamageNumUpdate();
    BtlHudGaugeUpdate();
    BtlHudComboPopupUpdate(btlHudEnemyComboPopups);
    BtlHudComboPopupUpdate(btlHudPartyComboPopups);
    BtlHudStatusIconUpdate();
    BtlHudStatChangeUpdate();
    BtlHudMarkerUpdate();
}
