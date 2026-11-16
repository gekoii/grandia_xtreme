#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "sys.h"


void BtlHudComboPopupDrawHits(BtlHudComboPopup *p) {
    s32 i;
    s32 n;

    SprBegin(0.0f, 0.0f);
    for (i = 0; i < 12; i++, p++) {
        if (p->rank > 0) {
            n = p->hits;
            sprState.ox = p->x;
            sprState.oy = p->y;
            if (n >= 2) {
                sprState.offX = 0;
                MenuNumberDraw(&btlHudPopupHitsNumFmt, n);
                sprState.offX = p->hitsWidth;
                SprDrawRect(&btlHudHitsRect, 4, -0x18);
            }
        }
    }
    SprFlush();
}
