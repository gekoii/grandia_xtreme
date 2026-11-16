#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"


void BtlHudComboPopupDrawDamage(BtlHudComboPopup *p) {
    s32 i;

    SprBegin(0.0f, 0.0f);
    for (i = 0; i < 12; i++, p++) {
        if (p->rank > 0) {
            sprState.ox = p->x;
            sprState.oy = p->y;
            if (p->hits >= 2) {
                sprState.offX = p->hitsWidth + 14.0f;
            } else {
                sprState.offX = 0;
            }
            MenuNumberDraw(&btlHudPopupDamageNumFmt, p->damage);
            sprState.offX += p->damageWidth;
            SprDrawRect(&btlHudPopupDamageRect, 4, -4);
        }
    }
    SprFlush();
}
