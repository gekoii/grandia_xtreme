/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n33.h"

f32 VuSinAngle(s32);

void BtlHudComboPopupUpdate(BtlHudComboPopup *p) {
    BtlHudComboPopup *q;
    s32 i;
    s32 j;
    f32 tgt;
    f32 t;

    for (i = 0; i < 12; i++, p++) {
        if (p->rank == 0) {
            continue;
        }
        if (++p->timer > 0x80) {
            if (p->unit >= 4) {
                q = btlHudEnemyComboPopups;
            } else {
                q = btlHudPartyComboPopups;
            }
            for (j = 0; j < 12; j++, q++) {
                if (p->rank < q->rank) {
                    q->rank--;
                }
            }
            p->rank = 0;
        } else {
            tgt = p->rank * 36.0f + 80.0f;
            if (tgt < p->destY) {
                p->destY -= 6.0f;
                if (p->destY < tgt) {
                    p->destY = tgt;
                }
            } else if (p->destY < tgt) {
                p->destY += 6.0f;
                if (tgt < p->destY) {
                    p->destY = tgt;
                }
            }
            if (p->timer <= 0x10) {
                t = VuSinAngle(p->timer << 10);
                p->x = (p->destX - p->startX) * t + p->startX;
                p->y = (p->destY - p->startY) * t + p->startY;
            } else {
                p->x = p->destX;
                p->y = p->destY;
            }
        }
    }
}
