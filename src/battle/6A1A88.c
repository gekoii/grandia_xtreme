/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n30.h"

void FxFailLabelUpdate(BtlUnit *arg0) {
    FxFailLabel *p = &fxFailLabel[arg0->id];

    if (p->timer >= 0) {
        if (p->timer == 8 || p->timer == 16) {
            p->frame++;
        }
        if (p->y > 0.0f) {
            p->y += p->vy;
            if (p->vy > -3.0f) {
                p->vy -= 0.2f;
            }
        }
        p->timer++;
        if (p->timer > 0x70) {
            p->timer = -1;
        }
    }
}
