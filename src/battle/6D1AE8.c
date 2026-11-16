#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"


void BtlHudComboPopupDrawIcon(BtlHudComboPopup *p) {
    s32 i;
    s32 n;

    SprBegin(0.0f, 0.0f);
    for (i = 0; i < 12; i++, p++) {
        if (p->rank > 0) {
            n = p->unit;
            sprState.ox = p->x;
            sprState.oy = p->y;
            if (n >= 4) {
                BtlUnitIconDraw(&btlUnits[n], -0x10, -4, -0x2000);
            } else {
                BtlUnitIconDraw(&btlUnits[n], p->hitsWidth + 14.0f + p->damageWidth + 56.0f + 16.0f, -4, -0x2000);
            }
        }
    }
    SprFlush();
}
