#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"


void BtlHudDmgDraw(void) {
    GphTexBind(0xE9);
    if (btlActingUnit >= 0x10 || btlUnits[btlActingUnit].state != 0x81) {
        BtlHudGaugeDraw();
    }
    BtlHudDamageNumDraw();
    BtlHudStatChangeDraw();
    BtlHudMarkerDraw();
    BtlHudComboPopupDrawHits(btlHudEnemyComboPopups);
    BtlHudComboPopupDrawHits(btlHudPartyComboPopups);
    GphTexBind(0xEB);
    BtlHudComboPopupDrawDamage(btlHudEnemyComboPopups);
    BtlHudComboPopupDrawDamage(btlHudPartyComboPopups);
    GphTexBind(0xEA);
    BtlHudComboPopupDrawIcon(btlHudEnemyComboPopups);
    BtlHudComboPopupDrawIcon(btlHudPartyComboPopups);
    GphTexBind(0xEC);
    BtlHudStatusIconDraw();
}
